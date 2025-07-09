/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 03:02:33 by hawayda           #+#    #+#             */
/*   Updated: 2025/07/09 23:13:07 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_rules	t_rules;

typedef struct s_philo
{
	int					id;
	long				last_meal;
	int					meals;
	pthread_t			thread;
	pthread_mutex_t		*l_fork;
	pthread_mutex_t		*r_fork;
	pthread_mutex_t		lock;
	t_rules				*rules;
}						t_philo;

struct					s_rules
{
	int					n_philo;
	int					t_die;
	int					t_eat;
	int					t_sleep;
	int					must_eat;
	long				start;
	bool				stop;
	pthread_mutex_t		print;
	pthread_mutex_t		sim_lock;
	pthread_mutex_t		*forks;
	t_philo				*philos;
};

bool					error_out(const char *msg);
bool					get_bool(t_mutex *mutex, bool *value);
bool					simulation_finished(t_table *table);
bool					all_threads_running(t_mutex *mutex, long *threads,
							long philo_nbr);

int						ft_atoi_pos(const char *s, bool *ok);

long					get_time_ms(void);

bool					parse_input(t_table *table, char **av);
void					safe_thread_handler(pthread_t *thread,
							void *(*f)(void *), void *data, t_opcode opcode);
void					data_init(t_table *table);
void					safe_mutex_handler(t_mutex *mutex, t_opcode opcode);
void					safe_thread_handler(pthread_t *thread,
							void *(*f)(void *), void *data, t_opcode opcode);
void					dinner_start(t_table *table);
void					set_bool(t_mutex *mutex, bool *dest, bool value);
void					set_long(t_mutex *mutex, long *dest, long value);
void					precise_usleep(t_table *table, long usec);
void					write_status(t_philo *philo, t_philo_status status,
							bool debug);
void					increase_long(t_mutex *mutex, long *value);
void					philo_think(t_philo *philo, bool pre_simulation);
void					desyncrhonize_philos(t_philo *philo);

void					*safe_malloc(size_t size);
void					*monitor_dinner(void *data);

#endif
