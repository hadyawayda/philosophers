/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 18:15:11 by hawayda           #+#    #+#             */
/*   Updated: 2025/07/09 23:13:04 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	clean(t_table *table)
{
	bool	ok;
	long	tmp;

	if (ac != 5 && ac != 6)
		return (false);
	ok = true;
	tmp = ft_atoi_pos(av[1], &ok);
	r->n_philo = (int)tmp;
	r->t_die = (int)ft_atoi_pos(av[2], &ok);
	r->t_eat = (int)ft_atoi_pos(av[3], &ok);
	r->t_sleep = (int)ft_atoi_pos(av[4], &ok);
	r->must_eat = -1;
	if (ac == 6)
		r->must_eat = (int)ft_atoi_pos(av[5], &ok);
	if (!ok || !valid_int(r->n_philo) || !valid_int(r->t_die)
		|| !valid_int(r->t_eat) || !valid_int(r->t_sleep) || (ac == 6
			&& !valid_int(r->must_eat)))
		return (false);
	return (true);
}

static int	one_philo_case(t_rules *r)
{
	long	ts;

	ts = 0;
	printf("%ld %d has taken a fork\n", ts, 1);
	ft_usleep(r->t_die);
	ts = r->t_die;
	printf("%ld %d died\n", ts, 1);
	return (0);
}

static void	wait_end(t_rules *r)
{
	int	i;

	monitor(r);
	i = -1;
	while (++i < r->n_philo)
		pthread_join(r->philos[i].thread, NULL);
}

static void	assign_forks(t_philo *philo, t_fork *forks, int position)
{
	int	philo_nbr;

	philo_nbr = philo->table->philo_nbr;
	philo->first_fork = &forks[(position + 1) % philo_nbr];
	philo->second_fork = &forks[position];
	if (philo->id % 2 == 0)
	{
		philo->first_fork = &forks[position];
		philo->second_fork = &forks[(position + 1) % philo_nbr];
	}
}

static void	philo_init(t_table *table)
{
	int		i;
	t_philo	*philo;

	i = -1;
	while (++i < table->philo_nbr)
	{
		philo = &table->philos[i];
		philo->id = i + 1;
		philo->full = false;
		philo->meals_counter = 0;
		philo->table = table;
		safe_mutex_handler(&philo->philo_mutex, INIT);
		assign_forks(philo, table->forks, i);
	}
}

void	data_init(t_table *table)
{
	int	i;

	i = -1;
	table->end_simulation = false;
	table->all_threads_ready = false;
	table->threads_running_nbr = 0;
	table->philos = safe_malloc(sizeof(t_philo) * table->philo_nbr);
	safe_mutex_handler(&table->table_mutex, INIT);
	safe_mutex_handler(&table->write_mutex, INIT);
	table->forks = safe_malloc(sizeof(t_fork) * table->philo_nbr);
	while (++i < table->philo_nbr)
	{
		safe_mutex_handler(&table->forks[i].fork, INIT);
		table->forks[i].fork_id = i;
	}
	philo_init(table);
}

int	main(int ac, char **av)
{
	t_rules	r;

	if (ac != 5 && ac != 6)
		return (error_out("Error: invalid arguments\n" G \
			"Correct usage: ./philo 5 800 200 200 [5]" RST), -1);
	if (!parse_input(&table, av))
    	return (1);
	data_init(&table);
	dinner_start(&table);
	clean(&table);
	return (0);
}
