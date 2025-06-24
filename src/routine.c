/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 15:18:14 by hawayda           #+#    #+#             */
/*   Updated: 2025/06/24 22:02:42 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	eat(t_philo *p)
{
	pthread_mutex_lock(p->first_fork);
	// log_state(p, "has taken a fork", false);
	pthread_mutex_lock(p->second_fork);
	// log_state(p, "has taken a fork", false);
	pthread_mutex_lock(&p->lock);
	// p->last_meal_time = get_time_ms();
	pthread_mutex_unlock(&p->lock);
	// log_state(p, "is eating", false);
	// ft_usleep(p->table->time_to_eat);
	pthread_mutex_lock(&p->lock);
	p->meals_counter++;
	pthread_mutex_unlock(&p->lock);
	pthread_mutex_unlock(p->second_fork);
	pthread_mutex_unlock(p->first_fork);
}

void	*routine(void *arg)
{
	t_philo	*p;

	p = arg;
	if (p->table->philo_nbr == 1)
		return (NULL); /* handled in main */
	if (p->id % 2 == 0)
		usleep(1000);
	while (true)
	{
		pthread_mutex_lock(&p->table->sim_lock);
		if (p->table->end_simulation)
			return (pthread_mutex_unlock(&p->table->sim_lock), NULL);
		pthread_mutex_unlock(&p->table->sim_lock);
		eat(p);
		// log_state(p, "is sleeping", false);
		ft_usleep(p->table->time_to_sleep);
		// log_state(p, "is thinking", false);
	}
	return (NULL);
}

static void	set_philo(t_table *table, int i)
{
	t_philo	*p;

	p = &table->philos[i];
	p->id = i + 1;
	p->last_meal_time = table->start_simulation;
	p->meals_counter = 0;
	p->first_fork = &table->forks[i].fork;
	p->second_fork = &table->forks[(i + 1) % table->philo_nbr].fork;
	pthread_mutex_init(&p->lock, NULL);
	p->table = table;
}

static bool	create_threads(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->philo_nbr)
	{
		if (pthread_create(&table->philos[i].thread_id, NULL, routine,
				&table->philos[i]))
			return (false);
		i += 2;
		if (i >= table->philo_nbr)
			i = 1;
	}
	return (true);
}

bool	init_sim(t_table *table)
{
	int	i;

	// table->start_simulation = get_time_ms();
	table->end_simulation = false;
	pthread_mutex_init(&table->print, NULL);
	pthread_mutex_init(&table->sim_lock, NULL);
	table->forks = malloc(sizeof(pthread_mutex_t) * table->philo_nbr);
	table->philos = malloc(sizeof(t_philo) * table->philo_nbr);
	if (!table->forks || !table->philos)
		return (false);
	i = -1;
	while (++i < table->philo_nbr)
		pthread_mutex_init(&table->forks[i].fork, NULL);
	i = -1;
	while (++i < table->philo_nbr)
		set_philo(table, i);
	if (!create_threads(table))
		return (false);
	return (true);
}
