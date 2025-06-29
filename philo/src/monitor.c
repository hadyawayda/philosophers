/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 03:05:02 by hawayda           #+#    #+#             */
/*   Updated: 2025/06/29 03:05:02 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	all_fed(t_rules *r)
{
	int	i;
	int	full;

	if (r->must_eat < 0)
		return (false);
	i = 0;
	full = 0;
	while (i < r->n_philo)
	{
		pthread_mutex_lock(&r->philos[i].lock);
		if (r->philos[i].meals >= r->must_eat)
			full++;
		pthread_mutex_unlock(&r->philos[i].lock);
		i++;
	}
	return (full == r->n_philo);
}

static bool	check_death(t_rules *r, int i)
{
	long	now;
	long	last;
	bool	first;

	pthread_mutex_lock(&r->philos[i].lock);
	last = r->philos[i].last_meal;
	pthread_mutex_unlock(&r->philos[i].lock);
	now = get_time_ms();
	if (now - last >= r->t_die)
	{
		pthread_mutex_lock(&r->sim_lock);
		first = !r->stop;
		r->stop = true;
		pthread_mutex_unlock(&r->sim_lock);
		if (first)
			log_state(&r->philos[i], "died", true);
		return (true);
	}
	return (false);
}

static bool	check_all(t_rules *r)
{
	int	i;

	i = 0;
	while (i < r->n_philo)
		if (check_death(r, i++))
			return (true);
	if (all_fed(r))
	{
		pthread_mutex_lock(&r->sim_lock);
		r->stop = true;
		pthread_mutex_unlock(&r->sim_lock);
		return (true);
	}
	return (false);
}

void	monitor(t_rules *r)
{
	while (1)
	{
		pthread_mutex_lock(&r->sim_lock);
		if (r->stop)
		{
			pthread_mutex_unlock(&r->sim_lock);
			break ;
		}
		pthread_mutex_unlock(&r->sim_lock);
		if (check_all(r))
			return ;
		usleep(100);
	}
}
