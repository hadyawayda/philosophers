/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 10:22:48 by hawayda           #+#    #+#             */
/*   Updated: 2025/06/24 22:04:26 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	simulation_finished(t_table *table)
{
	return (get_bool(&table->table_mutex, &table->end_simulation));
}

static void	philo_eat(t_philo *philo)
{
	safe_mutex_handler(&philo->first_fork->fork, LOCK);
	write_status(philo, TAKE_FIRST_FORK, DEBUG_MODE);
	safe_mutex_handler(&philo->second_fork->fork, LOCK);
	write_status(philo, TAKE_SECOND_FORK, DEBUG_MODE);
	set_long(philo->philo_mutex, &philo->last_meal_time,
		get_time_ms(MILLISECOND));
	philo->meals_counter += 1;
	write_status(philo, EATING, DEBUG_MODE);
	precise_usleep(philo->table, philo->table->time_to_eat);
	if (philo->table->meals_limit > 0
		&& philo->meals_counter == philo->table->meals_limit)
		set_bool(philo->philo_mutex, &philo->full, true);
}

void	*dinner_simulation(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	wait_all_threads(philo->table);
	while (!simulation_finished(philo->table))
	{
		if (philo->full)
			break ;
		philo_eat(philo);
		write_status(philo, SLEEPING, DEBUG_MODE);
		precise_usleep(philo->table, philo->table->time_to_sleep);
		// philo_think(philo);
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
		return ;
	else
	{
		while (++i < table->philo_nbr)
			safe_thread_handler(&table->philos[i].thread_id, dinner_simulation,
				&table->philos[i], CREATE);
	}
	table->start_simulation = get_time_ms(MILLISECOND);
	set_bool(&table->table_mutex, &table->all_threads_ready, true);
	i = -1;
	while (++i < table->philo_nbr)
		safe_thread_handler(&table->philos[i].thread_id, NULL, NULL, JOIN);
}
