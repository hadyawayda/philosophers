/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 17:51:31 by hawayda           #+#    #+#             */
/*   Updated: 2025/07/09 23:50:21 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** Returns true when *str is a non-empty string of decimal digits.
*/
static bool	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (false);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
		i++;
	}
	return (true);
}

long	ft_atol(const char *str)
{
	long	num;
	int		i;

	num = 0;
	i = 0;
	while (str[i])
	{
		num = num * 10 + (str[i] - '0');
		i++;
	}
	return (num);
}

static bool	get_long2(const char *arg, long *dst, const char *err)
{
	long	val;

	if (!is_numeric(arg))
		return (error_out(err));
	val = ft_atol(arg);
	if (val < 1 || val > INT_MAX)
		return (error_out(err));
	*dst = val;
	return (true);
}

static bool	set_time(long *field, const char *arg, const char *err)
{
	long	val;

	if (!get_long2(arg, &val, err))
		return (false);
	*field = val * 1000;
	return (true);
}

/*
** Parse all mandatory/optional CLI arguments and fill *table.
*/
bool	parse_input(t_table *table, char **av)
{
	long	tmp;

	if (!get_long2(av[1], &tmp, "Invalid input"))
		return (false);
	table->philo_nbr = (int)tmp;
	if (!set_time(&table->time_to_die, av[2], "Invalid input")
		|| !set_time(&table->time_to_eat, av[3], "Invalid input")
		|| !set_time(&table->time_to_sleep, av[4], "Invalid input"))
		return (false);
	if (table->time_to_die < 60000 || table->time_to_eat < 60000
		|| table->time_to_sleep < 60000)
		return (error_out("timestamps must be at least 60 ms"));
	if (av[5])
	{
		if (!get_long2(av[5], &tmp, "meals_limit"))
			return (false);
		table->meals_limit = (int)tmp;
	}
	else
		table->meals_limit = -1;
	return (true);
}
