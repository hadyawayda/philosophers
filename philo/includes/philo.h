/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 03:02:33 by hawayda           #+#    #+#             */
/*   Updated: 2025/06/29 03:02:59 by hawayda          ###   ########.fr       */
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

bool					parse_args(int ac, char **av, t_rules *r);
bool					init_sim(t_rules *r);

int						ft_atoi_pos(const char *s, bool *ok);

long					get_time_ms(void);

void					smart_sleep(long dur, t_rules *r);
void					think(t_philo *p);
void					eat(t_philo *p);
void					*routine(void *arg);
void					monitor(t_rules *r);
void					ft_usleep(long ms);
void					log_state(t_philo *p, const char *msg, bool death);
void					free_all(t_rules *r);

#endif
