/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:24:18 by hawayda           #+#    #+#             */
/*   Updated: 2025/06/24 22:02:55 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	write_status_debug(t_philo *philo, t_philo_status status,
		long elapsed)
{
	if ((TAKE_FIRST_FORK == status || TAKE_SECOND_FORK == status)
		&& !sumulation_finished(philo->table))
		printf(W "%-6ld" RST " %d has taken a fork\n", elapsed, philo->id);
	else if (EATING == status && !sumulation_finished(philo->table))
		printf(W "%-6ld" C " %d is eating\n" RST, elapsed, philo->id);
	else if (SLEEPING == status && !sumulation_finished(philo->table))
		printf(W "%-6ld" RST " %d is sleeping\n" RST, elapsed, philo->id);
	else if (THINKING == status && !sumulation_finished(philo->table))
		printf(W "%-6ld" RST " %d is thinking\n" RST, elapsed, philo->id);
	else if (DIED == status && !sumulation_finished(philo->table))
		printf(RED "%-6ld" RST " %d died\n" RST, elapsed, philo->id);
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
		if ((TAKE_FIRST_FORK == status || TAKE_SECOND_FORK == status)
			&& !sumulation_finished(philo->table))
			printf(W "%-6ld" RST " %d has taken a fork\n", elapsed, philo->id);
		else if (EATING == status && !sumulation_finished(philo->table))
			printf(W "%-6ld" C " %d is eating\n" RST, elapsed, philo->id);
		else if (SLEEPING == status && !sumulation_finished(philo->table))
			printf(W "%-6ld" RST " %d is sleeping\n" RST, elapsed, philo->id);
		else if (THINKING == status && !sumulation_finished(philo->table))
			printf(W "%-6ld" RST " %d is thinking\n" RST, elapsed, philo->id);
		else if (DIED == status && !sumulation_finished(philo->table))
			printf(RED "%-6ld" RST " %d died\n" RST, elapsed, philo->id);
	}
	safe_mutex_handler(&philo->table->write_mutex, UNLOCK);
}
