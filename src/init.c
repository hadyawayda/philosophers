/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 15:17:30 by hawayda           #+#    #+#             */
/*   Updated: 2025/06/17 15:17:30 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void set_philo(t_rules *r, int i)
{
    t_philo *p = &r->philos[i];

    p->id        = i + 1;
    p->last_meal = r->start;
    p->meals     = 0;
    p->l_fork    = &r->forks[i];
    p->r_fork    = &r->forks[(i + 1) % r->n_philo];
    pthread_mutex_init(&p->lock, NULL);
    p->rules     = r;
}

static bool create_threads(t_rules *r)
{
    int i = 0;

    while (i < r->n_philo)
    {
        if (pthread_create(&r->philos[i].thread, NULL,
                routine, &r->philos[i]))
            return (false);
        i += 2;
        if (i >= r->n_philo)
            i = 1;
    }
    return (true);
}

bool init_sim(t_rules *r)
{
    int i;

    r->start = get_time_ms();
    r->stop  = false;
    pthread_mutex_init(&r->print, NULL);
    pthread_mutex_init(&r->sim_lock, NULL);
    r->forks  = malloc(sizeof(pthread_mutex_t) * r->n_philo);
    r->philos = malloc(sizeof(t_philo) * r->n_philo);
    if (!r->forks || !r->philos)
        return (false);
    i = -1;
    while (++i < r->n_philo)
        pthread_mutex_init(&r->forks[i], NULL);
    i = -1;
    while (++i < r->n_philo)
        set_philo(r, i);
    if (!create_threads(r))
        return (false);
    return (true);
}