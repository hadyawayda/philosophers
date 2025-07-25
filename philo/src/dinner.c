/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42beirut.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 12:34:59 by hawayda           #+#    #+#             */
/*   Updated: 2025/07/24 12:34:59 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_think(t_philo *philo, bool pre_simulation)
{
	long	t_eat;
	long	t_sleep;
	long	t_think;

	if (!pre_simulation)
		write_status(philo, THINKING, DEBUG_MODE);
	if (philo->table->philo_nbr % 2 == 0)
		return ;
	t_eat = philo->table->time_to_eat;
	t_sleep = philo->table->time_to_sleep;
	t_think = t_eat * 2 - t_sleep;
	if (t_think < 0)
		t_think = 0;
	precise_usleep(philo->table, t_think * 0.42);
}

void	lone_philo(t_philo *philo)
{
	t_table	*table;

	table = philo->table;
	table->start_simulation = get_time_ms(MILLISECOND);
	set_bool(&table->table_mutex, &table->all_threads_ready, true);
	write_status(philo, TAKE_FIRST_FORK, DEBUG_MODE);
	precise_usleep(table, table->time_to_die);
	write_status(philo, DIED, DEBUG_MODE);
	set_bool(&table->table_mutex, &table->end_simulation, true);
}

static void	philo_eat(t_philo *philo)
{
	safe_mutex_handler(&philo->first_fork->fork, LOCK);
	write_status(philo, TAKE_FIRST_FORK, DEBUG_MODE);
	safe_mutex_handler(&philo->second_fork->fork, LOCK);
	write_status(philo, TAKE_SECOND_FORK, DEBUG_MODE);
	set_long(&philo->philo_mutex, &philo->last_meal_time,
		get_time_ms(MILLISECOND));
	philo->meals_counter += 1;
	write_status(philo, EATING, DEBUG_MODE);
	precise_usleep(philo->table, philo->table->time_to_eat);
	if (philo->table->meals_limit > 0
		&& philo->meals_counter == philo->table->meals_limit)
		set_bool(&philo->philo_mutex, &philo->full, true);
	safe_mutex_handler(&philo->first_fork->fork, UNLOCK);
	safe_mutex_handler(&philo->second_fork->fork, UNLOCK);
}

void	*dinner_simulation(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	while (!get_bool(&philo->table->table_mutex,
			&philo->table->all_threads_ready))
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
