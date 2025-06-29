/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 03:15:45 by hawayda           #+#    #+#             */
/*   Updated: 2025/06/29 03:18:17 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	print_status(t_table *table, int id, const char *msg)
{
	sem_wait(table->print);
	printf("%ld %d %s\n", get_time_ms() - table->start_time, id, msg);
	sem_post(table->print);
}

static void	*monitor_routine(void *arg)
{
	t_philo	*ph;

	ph = arg;
	while (1)
	{
		if (get_time_ms() - ph->last_meal > ph->table->time_to_die)
		{
			print_status(ph->table, ph->id, "died");
			exit(1);
		}
		usleep(1000);
	}
	return (NULL);
}

static void	prepare_and_take_forks(t_table *table, t_philo *ph, int id)
{
	ph->table = table;
	ph->id = id;
	ph->last_meal = get_time_ms();
	ph->meals_eaten = 0;
	if (pthread_create(&ph->monitor, NULL, monitor_routine, ph) != 0)
		error_exit("pthread_create failed");
	pthread_detach(ph->monitor);
	sem_wait(table->forks);
	print_status(table, id, "has taken a fork");
	sem_wait(table->forks);
	print_status(table, id, "has taken a fork");
	ph->last_meal = get_time_ms();
	print_status(table, id, "is eating");
}

static void	eat_sleep_think_and_exit(t_table *table, t_philo *ph, int id)
{
	while (table->meals_limit < 0 || ph->meals_eaten < table->meals_limit)
	{
		precise_usleep(table->time_to_eat);
		ph->meals_eaten++;
		sem_post(table->forks);
		sem_post(table->forks);
		print_status(table, id, "is sleeping");
		precise_usleep(table->time_to_sleep);
		print_status(table, id, "is thinking");
	}
	exit(0);
}

void	philosopher_process(t_table *table, int id)
{
	t_philo	ph;

	prepare_and_take_forks(table, &ph, id);
	eat_sleep_think_and_exit(table, &ph, id);
}
