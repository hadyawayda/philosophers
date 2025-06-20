/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 15:19:37 by hawayda           #+#    #+#             */
/*   Updated: 2025/06/17 15:19:37 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long get_time_ms(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
}

void ft_usleep(long ms)
{
	long start_simulation = get_time_ms();

	while (get_time_ms() - start_simulation < ms)
		usleep(100);
}

void free_all(t_table *table)
{
	int i = 0;

	while (i < table->philo_nbr)
	{
		pthread_mutex_destroy(&table->forks[i].fork);
		pthread_mutex_destroy(&table->philos[i].lock);
		i++;
	}
	pthread_mutex_destroy(&table->print);
	pthread_mutex_destroy(&table->sim_lock);
	free(table->forks);
	free(table->philos);
}

void    error_exit(const char *error)
{
	printf(RED"%s\n"RESET, error);
	exit(EXIT_FAILURE);
}
