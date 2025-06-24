/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 15:13:25 by hawayda           #+#    #+#             */
/*   Updated: 2025/06/17 15:13:25 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <errno.h>
# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

// RST
# define RST "\033[0m"

// Bold (bright) colors
# define RED "\033[1;31m"
# define G "\033[1;32m"
# define Y "\033[1;33m"
# define B "\033[1;34m"
# define M "\033[1;35m"
# define C "\033[1;36m"
# define W "\033[1;37m"

/*
 * Write function macro
 */
# define DEBUG_MODE 0

/***
 * OPCODE for mutex | thread functions
 */
typedef enum e_opcode
{
	LOCK,
	UNLOCK,
	INIT,
	DESTROY,
	CREATE,
	JOIN,
	DETACH
}						t_opcode;

/**
 * CODES for gettime
 */
typedef enum e_time_code
{
	SECOND,
	MILLISECOND,
	MICROSECOND
}						t_time_code;

/**
 * PHILO States
 */
typedef enum e_status
{
	EATING,
	SLEEPING,
	THINKING,
	TAKE_FIRST_FORK,
	TAKE_SECOND_FORK,
	DIED
}						t_philo_status;

/*
 * Structures
 */
typedef pthread_mutex_t	t_mutex;

/*
 * Forward declaration
 */
typedef struct s_table	t_table;

/*
 * Fork
 */
typedef struct s_fork
{
	t_mutex				fork;
	int					fork_id;
}						t_fork;

/*
 * Philo
 */
typedef struct s_philo
{
	int					id;
	long				meals_counter;
	long				last_meal_time;
	bool				full;
	t_fork				*first_fork;
	t_fork				*second_fork;
	pthread_t			thread_id;
	t_mutex				philo_mutex;
	t_table				*table;
	t_mutex				lock;
}						t_philo;

/*
 * table
 */
struct					s_table
{
	long				philo_nbr;
	long				time_to_die;
	long				time_to_eat;
	long				time_to_sleep;
	long				meals_limit;
	long				must_eat;
	long				start_simulation;
	bool				all_threads_ready;
	bool				end_simulation;
	t_mutex				table_mutex;
	t_mutex				write_mutex;
	t_fork				*forks;
	t_philo				*philos;
	t_mutex				print;
	t_mutex				sim_lock;
};

bool					init_sim(t_table *table);
bool					get_bool(t_mutex *mutex, bool *value);
bool					simulation_finished(t_table *table);

int						one_philo_case(t_table *table);
int						main(int ac, char **av);

long					ft_atol_pos(const char *s);
long					get_time_ms(t_time_code time_code);
long					get_long(t_mutex *mutex, long *value);

void					wait_end(t_table *table);
void					*routine(void *arg);
void					monitor(t_table *table);
void					ft_usleep(long ms);
void					log_state(t_philo *p, const char *msg, bool death);
void					error_exit(const char *error);
void					parse_input(t_table *table, char **av);
void					safe_thread_handler(pthread_t *thread,
							void *(*f)(void *), void *data, t_opcode opcode);
void					data_init(t_table *table);
void					*safe_malloc(size_t size);
void					safe_mutex_handler(t_mutex *mutex, t_opcode opcode);
void					safe_thread_handler(pthread_t *thread,
							void *(*f)(void *), void *data, t_opcode opcode);
void					dinner_start(t_table *table);
void					set_bool(t_mutex *mutex, bool *dest, bool value);
void					set_long(t_mutex *mutex, long *dest, long value);
void					wait_all_threads(t_table *table);
void					precise_usleep(t_table *table, long usec);
void					write_status(t_philo *philo, t_philo_status status,
							bool debug);

#endif
