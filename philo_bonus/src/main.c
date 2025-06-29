/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 19:27:26 by hawayda           #+#    #+#             */
/*   Updated: 2025/06/29 03:20:24 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	init_semaphores(t_table *table)
{
	sem_unlink("/philo_forks");
	sem_unlink("/philo_print");
	table->forks = sem_open("/philo_forks", O_CREAT | O_EXCL, 0644,
			table->philo_nbr);
	table->print = sem_open("/philo_print", O_CREAT | O_EXCL, 0644, 1);
	if (table->forks == SEM_FAILED || table->print == SEM_FAILED)
		error_exit("Semaphore init failed");
}

static void	cleanup(t_table *table)
{
	sem_close(table->forks);
	sem_close(table->print);
	sem_unlink("/philo_forks");
	sem_unlink("/philo_print");
	free(table->pids);
}

static void	spawn_philosophers(t_table *table)
{
	int		i;
	pid_t	pid;

	i = 0;
	table->pids = malloc(sizeof(pid_t) * table->philo_nbr);
	if (!table->pids)
		error_exit("Malloc failed");
	while (i < table->philo_nbr)
	{
		pid = fork();
		if (pid < 0)
			error_exit("Fork failed");
		if (pid == 0)
			philosopher_process(table, i + 1);
		table->pids[i] = pid;
		i++;
	}
}

static void	reap_philosophers(t_table *table)
{
	int		finished;
	int		status;
	pid_t	pid;
	int		i;

	finished = 0;
	while (finished < table->philo_nbr)
	{
		pid = waitpid(-1, &status, 0);
		if (pid < 0)
			error_exit("waitpid failed");
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		{
			i = 0;
			while (i < table->philo_nbr)
			{
				kill(table->pids[i], SIGTERM);
				i++;
			}
			break ;
		}
		finished++;
	}
}

int	main(int argc, char **argv)
{
	t_table	table;

	parse_input(argc, argv, &table);
	init_semaphores(&table);
	table.start_time = get_time_ms();
	spawn_philosophers(&table);
	reap_philosophers(&table);
	cleanup(&table);
	return (0);
}
