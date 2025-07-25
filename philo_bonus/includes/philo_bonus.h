/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42beirut.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 12:35:52 by hawayda           #+#    #+#             */
/*   Updated: 2025/07/24 12:35:52 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <pthread.h>
# include <semaphore.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <unistd.h>

/*
** Color codes
*/
# define RST "\033[0m"
# define RED "\033[1;31m"
# define G "\033[1;32m"
# define Y "\033[1;33m"
# define B "\033[1;34m"
# define M "\033[1;35m"
# define C "\033[1;36m"
# define W "\033[1;37m"

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
}					t_philo_status;

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
	volatile int	dead;
	sem_t			dead_lock;
	sem_t			*forks;
	sem_t			*print;
	sem_t			*limit;
	long			start_time;
	pid_t			*pids;
}					t_table;

/*
** Per-philosopher state (in each child)
*/
typedef struct s_philo
{
	int				id;
	long			last_meal;
	int				meals_eaten;
	pthread_t		monitor;
	sem_t			meal_lock;
	t_table			*table;
}					t_philo;

int					get_dead(t_table *table);

long				get_time_ms(void);
long				time_since_last_meal(t_philo *ph);

void				precise_usleep(t_table *table, long ms);
void				print_status(t_table *table, int id, const char *msg);
void				philosopher_process(t_table *table, int id);
void				store_last_meal(t_philo *ph);
void				parse_input(int argc, char **argv, t_table *table);
void				error_out(const char *msg);
void				free_table_heap(t_table *table);
void				set_dead(t_table *table);
void				fatal_sig(int sig);
void				cleanup_parent(t_table *table);
void				*monitor_routine(void *arg);
void				spawn_philosophers(t_table *table);
void				kill_other_procs(t_table *table, pid_t except_pid);

#endif
