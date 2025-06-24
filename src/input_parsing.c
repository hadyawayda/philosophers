/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 17:51:31 by hawayda           #+#    #+#             */
/*   Updated: 2025/06/24 22:02:30 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static inline bool	is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

static const char	*validate_input(const char *str)
{
	int			len;
	const char	*num;

	len = 0;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '+')
		str++;
	else if (*str == '-')
		error_exit("Provide positive integers only\n");
	if (!is_digit(*str))
		error_exit("The input is not a correct digit\n");
	num = str;
	while (is_digit(*str))
		len++;
	if (len > 10)
		error_exit("The value is too big, INT_MAX is the limit");
	return (num);
}

long	ft_atol(const char *s)
{
	long	num;

	num = 0;
	s = validate_input(s);
	while (is_digit(*s))
		num = (num * 10) + (*s++ - '0');
	if (num > INT_MAX)
		error_exit("The value is too big, INT_MAX is the limit");
	if (*s != '\0')
		error_exit("The input is not a correct digit\n");
	return (num);
}

void	parse_input(t_table *table, char **av)
{
	table->philo_nbr = ft_atol(av[1]);
	table->time_to_die = ft_atol(av[2]) * 1000;
	table->time_to_eat = ft_atol(av[3]) * 1000;
	table->time_to_sleep = ft_atol(av[4]) * 1000;
	if (table->time_to_die < 60 || table->time_to_eat < 60
		|| table->time_to_sleep < 60)
		error_exit("The timestamps must be at least 60ms.\n");
	if (av[5])
		table->meals_limit = ft_atol(av[5]);
	else
		table->meals_limit = -1;
}
