/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 17:08:34 by hawayda           #+#    #+#             */
/*   Updated: 2025/07/05 17:08:34 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

t_philo	*g_philo = NULL;

/*
** Child receives SIGTERM from the parent when somebody else died.
** Clean the process exactly like the normal shutdown path, then _exit(0).
*/
void	fatal_sig(int sig)
{
	(void)sig;
	/* g_philo is a static pointer set in philosopher_process() */
	extern t_philo	*g_philo;

	if (!g_philo)
		_exit(0);
	pthread_cancel(g_philo->monitor);
	pthread_join(g_philo->monitor, NULL);
	sem_destroy(&g_philo->meal_lock);
	free_table_heap(g_philo->table);
	sem_close(g_philo->table->forks);
	sem_close(g_philo->table->print);
	_exit(0);
}

/*
 * Prints a timestamped status line, protected by a semaphore.
 */
void print_status(t_table *table, int id, const char *msg)
{
	sem_wait(table->print);
	printf("%ld %d %s\n",
	       get_time_ms() - table->start_time,
	       id,
	       msg);
	sem_post(table->print);
}

/*
** Thread started by every child process.
** It watches its own philosopher for starvation.
** It also terminates as soon as some other philosopher has died
** (table->dead becomes 1), so pthread_join() never blocks forever.
*/
static void	*monitor_routine(void *arg)
{
	t_philo	*ph;

	ph = (t_philo *)arg;
	while (!get_dead(ph->table))
	{
		if (time_since_last_meal(ph) > ph->table->time_to_die)
		{
			print_status(ph->table, ph->id, "died");
			set_dead(ph->table);
			break;
		}
		usleep(1000);
	}
	return (NULL);
}

/*
** Runs in each child (one per philosopher).
*/
void	philosopher_process(t_table *table, int id)
{
	t_philo	ph;
	int		ret;

	/* --- Special case: one philosopher deadlocks on 2 forks --- */
	if (table->philo_nbr == 1)
	{
		print_status(table, id, "has taken a fork");
		precise_usleep(table->time_to_die);
		print_status(table, id, "died");
		exit(1);
	}

	/* --- Normal initialization for >1 philosophers --- */
	ph.table = table;
	ph.id = id;
	ph.meals_eaten = 0;
	g_philo = &ph;
	signal(SIGTERM, fatal_sig);
	if (sem_init(&ph.meal_lock, 0, 1) != 0)
		error_out("sem_init failed");
	store_last_meal(&ph);

	ret = pthread_create(&ph.monitor, NULL, monitor_routine, &ph);
	if (ret != 0)
		error_out("pthread_create failed");

	/* --- Main loop: eat, sleep, think until death or meal limit --- */
	while (!get_dead(table)
	   && (table->meals_limit < 0
	    || ph.meals_eaten < table->meals_limit))
	{
		sem_wait(table->forks);
		print_status(table, id, "has taken a fork");
		sem_wait(table->forks);
		print_status(table, id, "has taken a fork");

		store_last_meal(&ph);
		print_status(table, id, "is eating");
		precise_usleep(table->time_to_eat);
		ph.meals_eaten++;

		sem_post(table->forks);
		sem_post(table->forks);

		print_status(table, id, "is sleeping");
		precise_usleep(table->time_to_sleep);
		print_status(table, id, "is thinking");
	}

	/* --- Clean up and exit with correct code --- */
	pthread_join(ph.monitor, NULL);
	sem_destroy(&ph.meal_lock);
	free_table_heap(table);
	sem_close(table->forks);
	sem_close(table->print);
	exit(get_dead(table) ? 1 : 0);
}
