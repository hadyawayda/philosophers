/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 03:04:28 by hawayda           #+#    #+#             */
/*   Updated: 2025/06/29 03:04:37 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	take_forks(t_philo *p)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	first = p->l_fork;
	second = p->r_fork;
	if (p->id % 2 == 0)
	{
		first = p->r_fork;
		second = p->l_fork;
	}
	pthread_mutex_lock(first);
	log_state(p, "has taken a fork", false);
	pthread_mutex_lock(second);
	log_state(p, "has taken a fork", false);
}

void	smart_sleep(long dur, t_rules *r)
{
	long	start;

	start = get_time_ms();
	while (get_time_ms() - start < dur)
	{
		pthread_mutex_lock(&r->sim_lock);
		if (r->stop)
		{
			pthread_mutex_unlock(&r->sim_lock);
			break ;
		}
		pthread_mutex_unlock(&r->sim_lock);
		usleep(100);
	}
}

void	think(t_philo *p)
{
	long	window;

	window = p->rules->t_die - (p->rules->t_eat + p->rules->t_sleep);
	if (window < 0)
		window = 0;
	else if (window > 600)
		window = 200;
	else
		window /= 2;
	log_state(p, "is thinking", false);
	smart_sleep(window, p->rules);
}

void	eat(t_philo *p)
{
	take_forks(p);
	pthread_mutex_lock(&p->lock);
	p->last_meal = get_time_ms();
	pthread_mutex_unlock(&p->lock);
	log_state(p, "is eating", false);
	ft_usleep(p->rules->t_eat);
	pthread_mutex_lock(&p->lock);
	p->meals++;
	pthread_mutex_unlock(&p->lock);
	pthread_mutex_unlock(p->r_fork);
	pthread_mutex_unlock(p->l_fork);
}
