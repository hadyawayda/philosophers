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

# include <colors.h>
# include <errno.h>
# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

// Reset
#define ANSI_COLOR_RESET   "\033[0m"

// Bold (bright) colors
#define ANSI_BOLD_BLACK    "\033[1;30m"
#define ANSI_BOLD_RED      "\033[1;31m"
#define ANSI_BOLD_GREEN    "\033[1;32m"
#define ANSI_BOLD_YELLOW   "\033[1;33m"
#define ANSI_BOLD_BLUE     "\033[1;34m"
#define ANSI_BOLD_MAGENTA  "\033[1;35m"
#define ANSI_BOLD_CYAN     "\033[1;36m"
#define ANSI_BOLD_WHITE    "\033[1;37m"

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
}	t_opcode;

/*
 * Structures
*/
typedef pthread_mutex_t t_mutex;

/*
* Forward declaration
*/
typedef struct s_table t_table;

/*
 * Fork
*/
typedef struct s_fork
{
	t_mutex			fork;
	int				fork_id;
}					t_fork;

/*
 * Philo
*/
typedef struct s_philo
{
	int				id;
	long			meals_counter;
	bool			full;
	long			last_meal_time;
	pthread_t		thread_id;
	t_mutex			*left_fork;
	t_mutex			*right_fork;
	t_table			*table;
	t_mutex			lock;
}					t_philo;

/*
 * table
*/
struct s_table
{
	long			philo_nbr;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			meals_limit;
	long			must_eat;
	long			start_simulation;
	bool			end_simulation;
	t_fork			*forks;
	t_philo			*philos;
	t_mutex			print;
	t_mutex			sim_lock;
};

bool	init_sim(t_table *table);

int		one_philo_case(t_table *table);
int		main(int ac, char **av);

long	ft_atol_pos(const char *s);
long	get_time_ms(void);

void	wait_end(t_table *table);
void	*routine(void *arg);
void	monitor(t_table *table);
void	ft_usleep(long ms);
void	log_state(t_philo *p, const char *msg, bool death);
void	free_all(t_table *table);
void	error_exit(const char *error);
void	parse_input(t_table *table, char **av);
void	safe_thread_handler(pthread_t *thread, void *(*f)(void *), void *data, t_opcode opcode);

#endif
