/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 15:18:49 by hawayda           #+#    #+#             */
/*   Updated: 2025/06/17 15:18:49 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void log_state(t_philo *p, const char *msg, bool death)
{
    long ts;

    pthread_mutex_lock(&p->rules->print);
    pthread_mutex_lock(&p->rules->sim_lock);
    if (!p->rules->stop || death)
    {
        ts = get_time_ms() - p->rules->start;
        printf("%ld %d %s\n", ts, p->id, msg);
    }
    pthread_mutex_unlock(&p->rules->sim_lock);
    pthread_mutex_unlock(&p->rules->print);
}

static bool all_fed(t_rules *r)
{
    int i = 0;
    int full = 0;

    if (r->must_eat < 0)
        return (false);
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

static bool check_death(t_rules *r, int i)
{
    long now;
    bool first;

    pthread_mutex_lock(&r->philos[i].lock);
    now = get_time_ms();
    if (now - r->philos[i].last_meal >= r->t_die)
    {
        pthread_mutex_unlock(&r->philos[i].lock);
        pthread_mutex_lock(&r->sim_lock);
        first = !r->stop;
        r->stop = true;
        pthread_mutex_unlock(&r->sim_lock);
        if (first)
            log_state(&r->philos[i], "died", true);
        exit(0);
    }
    pthread_mutex_unlock(&r->philos[i].lock);
    return (false);
}

void monitor(t_rules *r)
{
    int i;

    while (true)
    {
        pthread_mutex_lock(&r->sim_lock);
        if (r->stop)
            exit(0);
        pthread_mutex_unlock(&r->sim_lock);
        i = 0;
        while (i < r->n_philo)
            check_death(r, i++);
        if (all_fed(r))
        {
            pthread_mutex_lock(&r->sim_lock);
            r->stop = true;
            pthread_mutex_unlock(&r->sim_lock);
            exit(0);
        }
        usleep(100);
    }
}
