/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 03:05:27 by hawayda           #+#    #+#             */
/*   Updated: 2025/06/29 03:07:49 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi_pos(const char *s, bool *ok)
{
	long	res;

	res = 0;
	while (*s == ' ' || (*s >= 9 && *s <= 13))
		s++;
	if (*s == '+')
		s++;
	if (*s < '0' || *s > '9')
		return (*ok = false, 0);
	while (*s >= '0' && *s <= '9')
	{
		res = res * 10 + (*s - '0');
		if (res > INT_MAX)
			return (*ok = false, 0);
		s++;
	}
	if (*s != '\0')
		return (*ok = false, 0);
	return ((int)res);
}

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_usleep(long ms)
{
	long	start;

	start = get_time_ms();
	while (get_time_ms() - start < ms)
		usleep(100);
}

void	log_state(t_philo *p, const char *msg, bool death)
{
	long	ts;

	pthread_mutex_lock(&p->rules->sim_lock);
	if (!p->rules->stop || death)
	{
		ts = get_time_ms() - p->rules->start;
		pthread_mutex_lock(&p->rules->print);
		printf("%ld %d %s\n", ts, p->id, msg);
		pthread_mutex_unlock(&p->rules->print);
	}
	pthread_mutex_unlock(&p->rules->sim_lock);
}

void	free_all(t_rules *r)
{
	int	i;

	i = -1;
	while (++i < r->n_philo)
	{
		pthread_mutex_destroy(&r->forks[i]);
		pthread_mutex_destroy(&r->philos[i].lock);
	}
	pthread_mutex_destroy(&r->print);
	pthread_mutex_destroy(&r->sim_lock);
	free(r->forks);
	free(r->philos);
}
