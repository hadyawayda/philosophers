/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 15:19:37 by hawayda           #+#    #+#             */
/*   Updated: 2025/07/01 23:45:24 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * Monitor busy waits until
 * all threads are not running
 */
bool	all_threads_running(t_mutex *mutex, long *threads, long philo_nbr)
{
	bool	ret;

	ret = false;
	safe_mutex_handler(mutex, LOCK);
	if (*threads == philo_nbr)
		ret = true;
	safe_mutex_handler(mutex, UNLOCK);
	if (!ret)
		usleep(1000);
	return (ret);
}

/*
 * Increase threads running
 * to synchronize with the monitor
 */
void	increase_long(t_mutex *mutex, long *value)
{
	safe_mutex_handler(mutex, LOCK);
	(*value)++;
	safe_mutex_handler(mutex, UNLOCK);
}

long	get_time_ms(t_time_code time_code)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (error_out("Error in get_time_ms"), -1);
	if (SECOND == time_code)
		return (tv.tv_sec + (tv.tv_usec / 1e6));
	else if (MILLISECOND == time_code)
		return (tv.tv_sec * 1e3 + tv.tv_usec / 1e3);
	else if (MICROSECOND == time_code)
		return (tv.tv_sec * 1e6 + tv.tv_usec);
	else
		return (error_out("Unknown time_code"), -1);
	return (1337);
}

void	precise_usleep(t_table *table, long usec)
{
	long	start;
	long	elapsed;
	long	remaining;
	long	MAX_CHUNK;

	start = get_time_ms(MICROSECOND);
	MAX_CHUNK = 1000000;
	while (get_time_ms(MICROSECOND) - start < usec)
	{
		if (simulation_finished(table))
			break ;
		elapsed = get_time_ms(MICROSECOND) - start;
		remaining = usec - elapsed;
		if (remaining > MAX_CHUNK)
			usleep(MAX_CHUNK);
		else
			usleep(remaining);
	}
}

bool error_out(const char *msg)
{
    printf(RED "%s\n" RST, msg);
    return (false);
}
