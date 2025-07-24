/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters_setters.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42beirut.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 12:35:15 by hawayda           #+#    #+#             */
/*   Updated: 2025/07/24 12:35:15 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_bool(t_mutex *mutex, bool *dest, bool value)
{
	safe_mutex_handler(mutex, LOCK);
	*dest = value;
	safe_mutex_handler(mutex, UNLOCK);
}

bool	get_bool(t_mutex *mutex, bool *value)
{
	bool	return_value;

	safe_mutex_handler(mutex, LOCK);
	return_value = *value;
	safe_mutex_handler(mutex, UNLOCK);
	return (return_value);
}

long	get_long(t_mutex *mutex, long *value)
{
	long	return_value;

	safe_mutex_handler(mutex, LOCK);
	return_value = *value;
	safe_mutex_handler(mutex, UNLOCK);
	return (return_value);
}

void	set_long(t_mutex *mutex, long *dest, long value)
{
	safe_mutex_handler(mutex, LOCK);
	*dest = value;
	safe_mutex_handler(mutex, UNLOCK);
}

bool	simulation_finished(t_table *table)
{
	return (get_bool(&table->table_mutex, &table->end_simulation));
}
