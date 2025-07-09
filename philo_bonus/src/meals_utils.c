/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meals_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 18:01:55 by hawayda           #+#    #+#             */
/*   Updated: 2025/07/10 01:43:35 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/* update last_meal under protection */
void	store_last_meal(t_philo *ph)
{
	sem_wait(&ph->meal_lock);
	ph->last_meal = get_time_ms();
	sem_post(&ph->meal_lock);
}

/* get time-since-meal in a race-free way */
long	time_since_last_meal(t_philo *ph)
{
	long	diff;

	sem_wait(&ph->meal_lock);
	diff = get_time_ms() - ph->last_meal;
	sem_post(&ph->meal_lock);
	return (diff);
}

void	cleanup_parent(t_table *table)
{
	sem_close(table->forks);
	sem_close(table->print);
	sem_unlink("/philo_forks");
	sem_unlink("/philo_print");
	sem_destroy(&table->dead_lock);
	free(table->pids);
}

void	free_table_heap(t_table *table)
{
	if (table->pids)
	{
		free(table->pids);
		table->pids = NULL;
	}
}

/*
** Spawn exactly table->philo_nbr children.
*/
void	spawn_philosophers(t_table *table)
{
	int		i;
	pid_t	pid;

	i = 0;
	while (i < table->philo_nbr)
	{
		pid = fork();
		if (pid < 0)
			error_out("fork failed");
		if (pid == 0)
			philosopher_process(table, i + 1);
		table->pids[i] = pid;
		i++;
	}
}
