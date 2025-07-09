/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 19:28:09 by hawayda           #+#    #+#             */
/*   Updated: 2025/07/10 00:11:45 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/*
 * Return current time in milliseconds.
 */
long	get_time_ms(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) < 0)
		error_out("gettimeofday failed");
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
}

/*
 * Sleep for approximately 'ms' milliseconds.
 */
void	precise_usleep(long ms)
{
	long	start;

	start = get_time_ms();
	while (get_time_ms() - start < ms)
		usleep(100);
}

/* mark table->dead = 1 under semaphore protection */
void	set_dead(t_table *table)
{
	sem_wait(&table->dead_lock);
	table->dead = 1;
	sem_post(&table->dead_lock);
}

/* read table->dead */
int	get_dead(t_table *table)
{
	int	val;

	sem_wait(&table->dead_lock);
	val = table->dead;
	sem_post(&table->dead_lock);
	return (val);
}
