/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 19:27:53 by hawayda           #+#    #+#             */
/*   Updated: 2025/07/10 00:12:09 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	error_out(const char *msg)
{
	printf("%s\n", msg);
	exit(EXIT_FAILURE);
}

void	parse_input(int argc, char **argv, t_table *table)
{
	long	tmp;

	if (argc != 5 && argc != 6)
		error_out("Usage: ./philo_bonus "
			"number_of_philosophers time_to_die time_to_eat "
			"time_to_sleep [nbr_of_meals]");
	table->philo_nbr = atoi(argv[1]);
	table->time_to_die = atol(argv[2]);
	table->time_to_eat = atol(argv[3]);
	table->time_to_sleep = atol(argv[4]);
	if (table->philo_nbr < 1 || table->time_to_die < 1 || table->time_to_eat < 1
		|| table->time_to_sleep < 1)
		error_out("Invalid arguments");
	if (argc == 6)
	{
		tmp = atol(argv[5]);
		if (tmp < 1)
			error_out("Invalid number_of_times_each_philosopher_must_eat");
		table->meals_limit = tmp;
	}
	else
		table->meals_limit = -1;
}

/*
** Child receives SIGTERM from the parent when somebody else died.
** Clean the process exactly like the normal shutdown path, then _exit(0).
*/
void	fatal_sig(int sig)
{
	extern t_philo	*g_philo;

	(void)sig;
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
void	print_status(t_table *table, int id, const char *msg)
{
	sem_wait(table->print);
	printf("%ld %d %s\n", get_time_ms() - table->start_time, id, msg);
	sem_post(table->print);
}

/*
** Thread started by every child process.
** It watches its own philosopher for starvation.
** It also terminates as soon as some other philosopher has died
** (table->dead becomes 1), so pthread_join() never blocks forever.
*/
void	*monitor_routine(void *arg)
{
	t_philo	*ph;

	ph = (t_philo *)arg;
	while (!get_dead(ph->table))
	{
		if (time_since_last_meal(ph) > ph->table->time_to_die)
		{
			print_status(ph->table, ph->id, "died");
			set_dead(ph->table);
			break ;
		}
		usleep(1000);
	}
	return (NULL);
}
