/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 19:23:48 by hawayda           #+#    #+#             */
/*   Updated: 2025/06/27 19:23:48 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <errno.h>
# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>
# include <sys/wait.h>
# include <signal.h>
# include <fcntl.h>
# include <semaphore.h>

/*
** Color codes
*/
# define RST "\033[0m"
# define RED "\033[1;31m"
# define G   "\033[1;32m"
# define Y   "\033[1;33m"
# define B   "\033[1;34m"
# define M   "\033[1;35m"
# define C   "\033[1;36m"
# define W   "\033[1;37m"

/*
** Philosopher status messages
*/
typedef enum e_philo_status
{
	TAKEN_FORK,
	EATING,
	SLEEPING,
	THINKING,
	DIED
}				t_philo_status;

/*
** Shared table data (in parent)
*/
typedef struct s_table
{
	int				philo_nbr;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				meals_limit;
	sem_t			*forks;      /* counting semaphore */
	sem_t			*print;      /* binary semaphore for stdout */
	long			start_time;  /* t0 in ms */
	pid_t			*pids;       /* array of child PIDs */
}				t_table;

/*
** Per-philosopher state (in each child)
*/
typedef struct s_philo
{
	t_table			*table;
	int				id;
	long			last_meal;   /* timestamp of last eat */
	int				meals_eaten;
	pthread_t		monitor;     /* death-watcher thread */
}				t_philo;

/*
** error handling
*/
void	error_out(const char *msg);

/*
** input parsing
*/
void	parse_input(int argc, char **argv, t_table *table);

/*
** timing
*/
long	get_time_ms(void);
void	precise_usleep(long ms);

/*
** output
*/
void	print_status(t_table *table, int id, const char *msg);

/*
** philosopher lifecycle
*/
void	philosopher_process(t_table *table, int id);

#endif
