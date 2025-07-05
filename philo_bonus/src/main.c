/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 17:36:40 by hawayda           #+#    #+#             */
/*   Updated: 2025/07/05 17:36:40 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	init_semaphores(t_table *table)
{
	sem_unlink("/philo_forks");
	sem_unlink("/philo_print");
	table->forks = sem_open("/philo_forks",
	                        O_CREAT | O_EXCL, 0644,
	                        table->philo_nbr);
	table->print = sem_open("/philo_print",
	                        O_CREAT | O_EXCL, 0644,
	                        1);
	if (table->forks == SEM_FAILED || table->print == SEM_FAILED)
		error_out("sem_open failed");
	if (sem_init(&table->dead_lock, 0, 1) != 0)
		error_out("sem_init dead_lock failed");
	table->dead = 0;
}

/*
** Spawn exactly table->philo_nbr children.
*/
static void	spawn_philosophers(t_table *table)
{
	int		i;
	pid_t	pid;

	i = 0;
	while (i < table->philo_nbr)
	{
		pid = fork();
		if (pid < 0)
			error_out("fork failed");
		if (pid == 0)
			philosopher_process(table, i + 1);
		table->pids[i] = pid;
		i++;
	}
}

/*
** Post twice-per-philosopher to unblock any sem_waits after death.
*/
static void	unblock_forks(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->philo_nbr * 2)
	{
		sem_post(table->forks);
		i++;
	}
}

/*
** Reap all children; on first non-zero exit, mark dead and wake sleepers.
*/
static void	reap_philosophers(t_table *table)
{
	int		remaining;
	pid_t	pid;
	int		status;

	remaining = table->philo_nbr;
	while (remaining > 0)
	{
		pid = waitpid(-1, &status, 0);
		if (pid == -1)
		{
			if (errno == EINTR)
				continue;
			if (errno == ECHILD)
				break;
			error_out("waitpid failed");
		}
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		{
			set_dead(table);
			unblock_forks(table);
		}
		remaining--;
	}
}

/*
** main(): parse, init, fork & reap, then clean up.
*/
int	main(int argc, char **argv)
{
	t_table	table;

	parse_input(argc, argv, &table);
	init_semaphores(&table);
	table.start_time = get_time_ms();
	table.pids = malloc(sizeof(pid_t) * table.philo_nbr);
	if (!table.pids)
		error_out("malloc failed");
	spawn_philosophers(&table);
	reap_philosophers(&table);
	cleanup_parent(&table);
	return (0);
}
