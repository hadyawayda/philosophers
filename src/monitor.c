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

    pthread_mutex_lock(&p->table->print);
    pthread_mutex_lock(&p->table->sim_lock);
    if (!p->table->end_simulation || death)
    {
        ts = get_time_ms() - p->table->start_simulation;
        printf("%ld %d %s\n", ts, p->id, msg);
    }
    pthread_mutex_unlock(&p->table->sim_lock);
    pthread_mutex_unlock(&p->table->print);
}

static bool all_fed(t_table *r)
{
    int i = 0;
    int full = 0;

    if (r->must_eat < 0)
        return (false);
    while (i < r->philo_nbr)
    {
        pthread_mutex_lock(&r->philos[i].lock);
        if (r->philos[i].meals_counter >= r->must_eat)
            full++;
        pthread_mutex_unlock(&r->philos[i].lock);
        i++;
    }
    return (full == r->philo_nbr);
}

static bool check_death(t_table *r, int i)
{
    long now;
    bool first;

    pthread_mutex_lock(&r->philos[i].lock);
    now = get_time_ms();
    if (now - r->philos[i].last_meal_time >= r->time_to_die)
    {
        pthread_mutex_unlock(&r->philos[i].lock);
        pthread_mutex_lock(&r->sim_lock);
        first = !r->end_simulation;
        r->end_simulation = true;
        pthread_mutex_unlock(&r->sim_lock);
        if (first)
            log_state(&r->philos[i], "died", true);
        exit(0);
    }
    pthread_mutex_unlock(&r->philos[i].lock);
    return (false);
}

void monitor(t_table *r)
{
    int i;

    while (true)
    {
        pthread_mutex_lock(&r->sim_lock);
        if (r->end_simulation)
            exit(0);
        pthread_mutex_unlock(&r->sim_lock);
        i = 0;
        while (i < r->philo_nbr)
            check_death(r, i++);
        if (all_fed(r))
        {
            pthread_mutex_lock(&r->sim_lock);
            r->end_simulation = true;
            pthread_mutex_unlock(&r->sim_lock);
            exit(0);
        }
        usleep(100);
    }
}
