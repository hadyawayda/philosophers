/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 15:19:51 by hawayda           #+#    #+#             */
/*   Updated: 2025/06/17 15:19:51 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void free_all(t_rules *r)
{
    int i = 0;

    while (i < r->n_philo)
    {
        pthread_mutex_destroy(&r->forks[i]);
        pthread_mutex_destroy(&r->philos[i].lock);
        i++;
    }
    pthread_mutex_destroy(&r->print);
    pthread_mutex_destroy(&r->sim_lock);
    free(r->forks);
    free(r->philos);
}
