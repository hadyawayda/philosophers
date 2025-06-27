/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 19:28:09 by hawayda           #+#    #+#             */
/*   Updated: 2025/06/27 19:28:09 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/*
 * Return current time in milliseconds.
 */
long get_time_ms(void)
{
	struct timeval tv;

	if (gettimeofday(&tv, NULL) < 0)
		error_exit("gettimeofday failed");
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
}

/*
 * Sleep for approximately 'ms' milliseconds.
 */
void precise_usleep(long ms)
{
	long start = get_time_ms();
	while (get_time_ms() - start < ms)
		usleep(100);
}
