/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 10:22:48 by hawayda           #+#    #+#             */
/*   Updated: 2025/07/09 23:13:05 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	done_eating(t_philo *p)
{
	bool	done;

	pthread_mutex_lock(&p->lock);
	done = (p->rules->must_eat > -1 && p->meals >= p->rules->must_eat);
	pthread_mutex_unlock(&p->lock);
	return (done);
}

void	lone_philo(t_philo *philo)
{
	t_table	*table;

	table = philo->table;
	table->start_simulation = get_time_ms(MILLISECOND);
	set_bool(&table->table_mutex,
			 &table->all_threads_ready,
			 true);
	write_status(philo, TAKE_FIRST_FORK, DEBUG_MODE);
	precise_usleep(table, table->time_to_die);
	write_status(philo, DIED, DEBUG_MODE);
	set_bool(&table->table_mutex,
			 &table->end_simulation,
			 true);
}

static void	philo_loop(t_philo *p)
{
	while (!should_stop(p))
	{
		eat(p);
		if (done_eating(p))
			break ;
		log_state(p, "is sleeping", false);
		smart_sleep(p->rules->t_sleep, p->rules);
		think(p);
	}
}

void	*routine(void *arg)
{
	t_philo	*p;

	philo = (t_philo *)data;
	while (!get_bool(&philo->table->table_mutex, &philo->table->all_threads_ready))
		usleep(1000);
	set_long(&philo->philo_mutex, &philo->last_meal_time,
		get_time_ms(MILLISECOND));
	increase_long(&philo->table->table_mutex,
		&philo->table->threads_running_nbr);
	desyncrhonize_philos(philo);
	while (!simulation_finished(philo->table))
	{
		if (philo->full)
			break ;
		philo_eat(philo);
		write_status(philo, SLEEPING, DEBUG_MODE);
		precise_usleep(philo->table, philo->table->time_to_sleep);
		philo_think(philo, false);
	}
	return (NULL);
}

void	dinner_start(t_table *table)
{
	int	i;

	i = -1;
	if (0 == table->meals_limit)
		return ;
	else if (1 == table->philo_nbr)
	{
		lone_philo(&table->philos[0]);
		return ;
	}
	else
	{
		while (++i < table->philo_nbr)
			safe_thread_handler(&table->philos[i].thread_id, dinner_simulation,
				&table->philos[i], CREATE);
		safe_thread_handler(&table->monitor, monitor_dinner, table, CREATE);
		table->start_simulation = get_time_ms(MILLISECOND);
		set_bool(&table->table_mutex, &table->all_threads_ready, true);
		i = -1;
		while (++i < table->philo_nbr)
			safe_thread_handler(&table->philos[i].thread_id, NULL, NULL, JOIN);
		set_bool(&table->table_mutex, &table->end_simulation, true);
		safe_thread_handler(&table->monitor, NULL, NULL, JOIN);
	}
}
