/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42beirut.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 12:35:59 by hawayda           #+#    #+#             */
/*   Updated: 2025/07/24 21:23:32 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	init_semaphores(t_table *table)
{
	int	fork_tokens;
	int	limit_tokens;

	sem_unlink("/philo_forks");
	sem_unlink("/philo_print");
	sem_unlink("/philo_dead");
	sem_unlink("/philo_limit");
	fork_tokens = table->philo_nbr;
	table->forks = sem_open("/philo_forks", O_CREAT | O_EXCL, 0644,
			fork_tokens);
	table->print = sem_open("/philo_print", O_CREAT | O_EXCL, 0644, 1);
	table->dead_lock = *sem_open("/philo_dead", O_CREAT | O_EXCL, 0644, 1);
	if (table->philo_nbr > 1)
		limit_tokens = table->philo_nbr / 2;
	else
		limit_tokens = 1;
	table->limit = sem_open("/philo_limit", O_CREAT | O_EXCL, 0644,
			limit_tokens);
	if (table->forks == SEM_FAILED || table->print == SEM_FAILED
		|| table->limit == SEM_FAILED)
		error_out("sem_open failed");
	if (sem_init(&table->dead_lock, 0, 1) != 0)
		error_out("sem_init dead_lock failed");
	table->dead = 0;
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

static void	reap_philosophers(t_table *table)
{
	pid_t	pid;
	int		status;
	int		remaining;

	remaining = table->philo_nbr;
	while (remaining > 0)
	{
		pid = waitpid(-1, &status, 0);
		if (pid == -1)
		{
			if (errno == EINTR)
				continue ;
			if (errno == ECHILD)
				break ;
			error_out("waitpid failed");
		}
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		{
			set_dead(table);
			unblock_forks(table);
			kill_other_procs(table, pid);
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
