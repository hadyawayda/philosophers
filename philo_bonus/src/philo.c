/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 17:08:34 by hawayda           #+#    #+#             */
/*   Updated: 2025/07/10 00:12:02 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

t_philo		*g_philo = NULL;

/*
** Special case: a single philosopher can only take one fork and then dies.
*/
static void	lone_philosopher(t_table *table, int id)
{
	print_status(table, id, "has taken a fork");
	precise_usleep(table->time_to_die);
	print_status(table, id, "died");
	exit(1);
}

/*
** Initialize the t_philo struct, set up semaphores and monitor thread.
*/
static void	init_philosopher(t_philo *ph, t_table *table, int id)
{
	int	ret;

	ph->table = table;
	ph->id = id;
	ph->meals_eaten = 0;
	g_philo = ph;
	signal(SIGTERM, fatal_sig);
	if (sem_init(&ph->meal_lock, 0, 1) != 0)
		error_out("sem_init failed");
	store_last_meal(ph);
	ret = pthread_create(&ph->monitor, NULL, monitor_routine, ph);
	if (ret != 0)
		error_out("pthread_create failed");
}

/*
** The main eat–sleep–think loop for each philosopher.
*/
static void	philosopher_loop(t_philo *ph)
{
	while (!get_dead(ph->table) && (ph->table->meals_limit < 0
			|| ph->meals_eaten < ph->table->meals_limit))
	{
		sem_wait(ph->table->forks);
		print_status(ph->table, ph->id, "has taken a fork");
		sem_wait(ph->table->forks);
		print_status(ph->table, ph->id, "has taken a fork");
		store_last_meal(ph);
		print_status(ph->table, ph->id, "is eating");
		precise_usleep(ph->table->time_to_eat);
		ph->meals_eaten++;
		sem_post(ph->table->forks);
		sem_post(ph->table->forks);
		print_status(ph->table, ph->id, "is sleeping");
		precise_usleep(ph->table->time_to_sleep);
		print_status(ph->table, ph->id, "is thinking");
	}
}

/*
** Clean up semaphores, join monitor, free and exit with correct status.
*/
static void	cleanup_and_exit(t_philo *ph)
{
	int	dead;

	pthread_join(ph->monitor, NULL);
	sem_destroy(&ph->meal_lock);
	free_table_heap(ph->table);
	sem_close(ph->table->forks);
	sem_close(ph->table->print);
	dead = get_dead(ph->table);
	if (dead != 0)
		exit(1);
	exit(0);
}

/*
** Entry point for each child process (one philosopher).
*/
void	philosopher_process(t_table *table, int id)
{
	t_philo	ph;
	int		n;

	n = table->philo_nbr;
	if (n == 1)
		lone_philosopher(table, id);
	init_philosopher(&ph, table, id);
	philosopher_loop(&ph);
	cleanup_and_exit(&ph);
}
