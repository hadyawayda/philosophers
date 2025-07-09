/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 10:58:40 by hawayda           #+#    #+#             */
/*   Updated: 2025/07/09 23:13:02 by hawayda          ###   ########.fr       */
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

static void	write_status_debug(t_philo *philo, t_philo_status status,
		long elapsed)
{
	if (TAKE_FIRST_FORK == status && !simulation_finished(philo->table))
		printf(W "%6ld" RST " %d has taken the 1st fork 🍴"
			"\t\t\t" B "[ 🥄 %d ]\n" RST, elapsed,
			philo->id, philo->first_fork->fork_id);
	else if (TAKE_SECOND_FORK == status && !simulation_finished(philo->table))
		printf(W "%6ld" RST " %d has taken the 2nd fork 🍴"
			"\t\t\t" B "[ 🥄 %d ]\n" RST, elapsed,
			philo->id, philo->second_fork->fork_id);
	else if (EATING == status && !simulation_finished(philo->table))
		printf(W "%6ld" C " %d is eating 🍝"
			"\t\t\t" Y "[🍝 %ld 🍝]\n" RST, elapsed,
			philo->id, philo->meals_counter);
	else if (SLEEPING == status && !simulation_finished(philo->table))
		printf(W "%6ld" RST " %d is sleeping 😴\n", elapsed, philo->id);
	else if (THINKING == status && !simulation_finished(philo->table))
		printf(W "%6ld" RST " %d is thinking 🤔\n", elapsed, philo->id);
	else if (DIED == status)
		printf(RED "\t💀💀💀 %6ld %d died 💀💀💀\n" RST, elapsed, philo->id);
}

void	write_status(t_philo *philo, t_philo_status status, bool debug)
{
	long	elapsed;

	elapsed = get_time_ms(MILLISECOND) - philo->table->start_simulation;
	if (philo->full)
		return ;
	safe_mutex_handler(&philo->table->write_mutex, LOCK);
	if (debug)
		write_status_debug(philo, status, elapsed);
	else
	{
		if ((status == TAKE_FIRST_FORK || status == TAKE_SECOND_FORK)
			&& !simulation_finished(philo->table))
			printf("%ld %d has taken a fork\n", elapsed, philo->id);
		else if (status == EATING && !simulation_finished(philo->table))
			printf("%ld %d is eating\n", elapsed, philo->id);
		else if (status == SLEEPING && !simulation_finished(philo->table))
			printf("%ld %d is sleeping\n", elapsed, philo->id);
		else if (status == THINKING && !simulation_finished(philo->table))
			printf("%ld %d is thinking\n", elapsed, philo->id);
		else if (status == DIED)
			printf("%ld %d died\n", elapsed, philo->id);
	}
	safe_mutex_handler(&philo->table->write_mutex, UNLOCK);
}

static bool	philo_died(t_philo *philo)
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
		usleep(1000);
		i = -1;
		while (++i < table->philo_nbr && !simulation_finished(table))
		{
			if (philo_died(table->philos + i))
			{
				set_bool(&table->table_mutex, &table->end_simulation, true);
				write_status(table->philos + i, DIED, DEBUG_MODE);
			}
		}
	}
}
