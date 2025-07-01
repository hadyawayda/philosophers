/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:24:18 by hawayda           #+#    #+#             */
/*   Updated: 2025/07/01 23:22:33 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
