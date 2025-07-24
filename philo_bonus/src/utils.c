/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42beirut.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 12:36:21 by hawayda           #+#    #+#             */
/*   Updated: 2025/07/24 12:36:21 by hawayda          ###   ########.fr       */
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
 * Sleep for approximately 'ms' milliseconds, but wake up immediately
 * if ANY philosopher has died (table->dead becomes non-zero).
 *
 * We break our sleep into 1 ms chunks so that a waiting thread
 * checks get_dead() at least once per millisecond.
 */
void	precise_usleep(t_table *table, long ms)
{
	long	start;
	long	elapsed;
	long	remaining;
	long	chunk;

	start = get_time_ms();
	while (get_time_ms() - start < ms)
	{
		if (get_dead(table))
			break ;
		elapsed = get_time_ms() - start;
		remaining = ms - elapsed;
		if (remaining > 1)
			chunk = 1;
		else
			chunk = remaining;
		usleep(chunk * 1000);
	}
}

/*
 * mark table->dead = 1 under semaphore protection
 */
void	set_dead(t_table *table)
{
	sem_wait(&table->dead_lock);
	table->dead = 1;
	sem_post(&table->dead_lock);
}

/*
 * read table->dead under semaphore protection
 */
int	get_dead(t_table *table)
{
	int	val;

	sem_wait(&table->dead_lock);
	val = table->dead;
	sem_post(&table->dead_lock);
	return (val);
}

/* kill_other_procs() : SIGTERM every child except the one that just died */
void	kill_other_procs(t_table *table, pid_t except_pid)
{
	int	i;

	i = 0;
	while (i < table->philo_nbr)
	{
		if (table->pids[i] != except_pid)
			kill(table->pids[i], SIGTERM);
		i++;
	}
}
