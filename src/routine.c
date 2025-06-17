/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 15:18:14 by hawayda           #+#    #+#             */
/*   Updated: 2025/06/17 15:18:14 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void eat(t_philo *p)
{
    pthread_mutex_lock(p->l_fork);
    log_state(p, "has taken a fork", false);
    pthread_mutex_lock(p->r_fork);
    log_state(p, "has taken a fork", false);
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

void *routine(void *arg)
{
    t_philo *p = arg;

    if (p->rules->n_philo == 1)
        return NULL; /* handled in main */
    if (p->id % 2 == 0)
        usleep(1000);
    while (true)
    {
        pthread_mutex_lock(&p->rules->sim_lock);
        if (p->rules->stop)
            return (pthread_mutex_unlock(&p->rules->sim_lock), NULL);
        pthread_mutex_unlock(&p->rules->sim_lock);
        eat(p);
        log_state(p, "is sleeping", false);
        ft_usleep(p->rules->t_sleep);
        log_state(p, "is thinking", false);
    }
    return (NULL);
}
