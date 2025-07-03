/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 19:27:53 by hawayda           #+#    #+#             */
/*   Updated: 2025/06/27 19:27:53 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void error_out(const char *msg)
{
	printf("%s\n", msg);
	exit(EXIT_FAILURE);
}

void parse_input(int argc, char **argv, t_table *table)
{
	long tmp;

	if (argc != 5 && argc != 6)
		error_out("Usage: ./philo_bonus "
		           "number_of_philosophers time_to_die time_to_eat "
		           "time_to_sleep [number_of_times_each_philosopher_must_eat]");

	table->philo_nbr   = atoi(argv[1]);
	table->time_to_die = atol(argv[2]);
	table->time_to_eat = atol(argv[3]);
	table->time_to_sleep = atol(argv[4]);

	if (table->philo_nbr < 1
	 || table->time_to_die < 1
	 || table->time_to_eat < 1
	 || table->time_to_sleep < 1)
		error_out("Invalid arguments");

	if (argc == 6)
	{
		tmp = atol(argv[5]);
		if (tmp < 1)
			error_out("Invalid number_of_times_each_philosopher_must_eat");
		table->meals_limit = tmp;
	}
	else
		table->meals_limit = -1;
}
