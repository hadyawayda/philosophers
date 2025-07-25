/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42beirut.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 12:35:27 by hawayda           #+#    #+#             */
/*   Updated: 2025/07/24 12:35:27 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	clean(t_table *table)
{
	t_philo	*philo;
	int		i;

	i = -1;
	while (++i < table->philo_nbr)
	{
		philo = table->philos + i;
		safe_mutex_handler(&philo->philo_mutex, DESTROY);
	}
	safe_mutex_handler(&table->write_mutex, DESTROY);
	safe_mutex_handler(&table->table_mutex, DESTROY);
	free(table->forks);
	free(table->philos);
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
	t_table	table;

	if (ac != 5 && ac != 6)
		return (error_out("Error: invalid arguments\n" G
				"Correct usage: ./philo 5 800 200 200 [5]" RST),
			-1);
	if (!parse_input(&table, av))
		return (1);
	data_init(&table);
	dinner_start(&table);
	clean(&table);
	return (0);
}
