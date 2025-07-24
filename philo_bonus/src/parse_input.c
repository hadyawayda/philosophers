/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 19:27:53 by hawayda           #+#    #+#             */
/*   Updated: 2025/07/10 01:27:45 by hawayda          ###   ########.fr       */
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
	(void)sig;
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
** Thread run inside each child-process.
** It watches its own philosopher for starvation.
** As soon as *any* philosopher has died, every monitor returns.
** The first monitor that notices starvation prints “died” and then
** terminates its entire process immediately with _exit(1); this lets
** the parent reap that child and SIGTERM the rest before they can print.
*/
void	*monitor_routine(void *arg)
{
	t_philo	*ph;
	long	interval;

	ph = (t_philo *)arg;
	while (!get_dead(ph->table))
	{
		/* read last_meal safely – but **only once** */
		sem_wait(&ph->meal_lock);
		interval = get_time_ms() - ph->last_meal;
		sem_post(&ph->meal_lock);
		if (interval > ph->table->time_to_die)
		{
			sem_wait(&ph->table->dead_lock);
			if (ph->table->dead == 0)
			{
				print_status(ph->table, ph->id, "died");
				ph->table->dead = 1;
			}
			sem_post(&ph->table->dead_lock);
			_exit(1);
		}
		usleep(1000);
	}
	return (NULL);
}
