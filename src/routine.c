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
