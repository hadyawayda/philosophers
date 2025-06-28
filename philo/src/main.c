/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 15:17:55 by hawayda           #+#    #+#             */
/*   Updated: 2025/06/29 02:46:57 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	desyncrhonize_philos(t_philo *philo)
{
	if (philo->table->philo_nbr % 2 == 0)
	{
		if (philo->id % 2 == 0)
			precise_usleep(philo->table, 3e4);
	}
	else
	{
		if (philo->id % 2)
			philo_think(philo, true);
	}
}

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

int	main(int ac, char **av)
{
	t_table	table;

	if (ac != 5 && ac != 6)
		error_exit("Error: invalid arguments\n" G \
			"Correct usage: ./philo 5 800 200 200 [5]" RST);
	parse_input(&table, av);
	data_init(&table);
	dinner_start(&table);
	clean(&table);
	return (0);
}
