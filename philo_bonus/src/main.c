/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 19:27:26 by hawayda           #+#    #+#             */
/*   Updated: 2025/06/27 19:27:26 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/*
 * Initialize named semaphores before forking.
 * We use two semaphores:
 *   "/philo_forks"  — counting semaphore, initial value = philo_nbr
 *   "/philo_print"  — binary semaphore = 1
 */
static void init_semaphores(t_table *table)
{
	sem_unlink("/philo_forks");
	sem_unlink("/philo_print");

	table->forks = sem_open("/philo_forks",
	                        O_CREAT | O_EXCL,
	                        0644,
	                        table->philo_nbr);
	table->print = sem_open("/philo_print",
	                        O_CREAT | O_EXCL,
	                        0644,
	                        1);
	if (table->forks == SEM_FAILED || table->print == SEM_FAILED)
		error_exit("Semaphore init failed");
}

/*
 * Clean up semaphores and allocated memory.
 */
static void cleanup(t_table *table)
{
	sem_close(table->forks);
	sem_close(table->print);
	sem_unlink("/philo_forks");
	sem_unlink("/philo_print");
	free(table->pids);
}

int main(int argc, char **argv)
{
	t_table table;
	int     i;
	int     finished = 0;
	int     status;

	parse_input(argc, argv, &table);
	init_semaphores(&table);
	table.start_time = get_time_ms();

	table.pids = malloc(sizeof(pid_t) * table.philo_nbr);
	if (!table.pids)
		error_exit("Malloc failed");

	/* spawn one process per philosopher */
	for (i = 0; i < table.philo_nbr; i++)
	{
		pid_t pid = fork();
		if (pid < 0)
			error_exit("Fork failed");
		if (pid == 0)
			philosopher_process(&table, i + 1);
		table.pids[i] = pid;
	}

	/* parent waits for children:
	 * - if any child exits with non-zero => a philosopher died:
	 *     kill all and stop
	 * - otherwise count normal exits (all ate enough) and exit
	 */
	while (finished < table.philo_nbr)
	{
		pid_t pid = waitpid(-1, &status, 0);
		if (pid < 0)
			error_exit("waitpid failed");
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		{
			/* someone died */
			for (i = 0; i < table.philo_nbr; i++)
				kill(table.pids[i], SIGTERM);
			break;
		}
		finished++;
	}

	cleanup(&table);
	return (0);
}
