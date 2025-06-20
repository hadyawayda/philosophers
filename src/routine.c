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
    pthread_mutex_lock(p->left_fork);
    log_state(p, "has taken a fork", false);
    pthread_mutex_lock(p->right_fork);
    log_state(p, "has taken a fork", false);
    pthread_mutex_lock(&p->lock);
    p->last_meal_time = get_time_ms();
    pthread_mutex_unlock(&p->lock);
    log_state(p, "is eating", false);
    ft_usleep(p->table->time_to_eat);
    pthread_mutex_lock(&p->lock);
    p->meals_counter++;
    pthread_mutex_unlock(&p->lock);
    pthread_mutex_unlock(p->right_fork);
    pthread_mutex_unlock(p->left_fork);
}

void *routine(void *arg)
{
    t_philo *p = arg;

    if (p->table->philo_nbr == 1)
        return NULL; /* handled in main */
    if (p->id % 2 == 0)
        usleep(1000);
    while (true)
    {
        pthread_mutex_lock(&p->table->sim_lock);
        if (p->table->end_simulation)
            return (pthread_mutex_unlock(&p->table->sim_lock), NULL);
        pthread_mutex_unlock(&p->table->sim_lock);
        eat(p);
        log_state(p, "is sleeping", false);
        ft_usleep(p->table->time_to_sleep);
        log_state(p, "is thinking", false);
    }
    return (NULL);
}

static void set_philo(t_table *r, int i)
{
    t_philo *p = &r->philos[i];

    p->id        = i + 1;
    p->last_meal_time = r->start_simulation;
    p->meals_counter = 0;
    p->left_fork    = &r->forks[i].fork;
    p->right_fork    = &r->forks[(i + 1) % r->philo_nbr].fork;
    pthread_mutex_init(&p->lock, NULL);
    p->table     = r;
}

static bool create_threads(t_table *r)
{
    int i = 0;

    while (i < r->philo_nbr)
    {
        if (pthread_create(&r->philos[i].thread_id, NULL,
                routine, &r->philos[i]))
            return (false);
        i += 2;
        if (i >= r->philo_nbr)
            i = 1;
    }
    return (true);
}

bool init_sim(t_table *r)
{
    int i;

    r->start_simulation = get_time_ms();
    r->end_simulation  = false;
    pthread_mutex_init(&r->print, NULL);
    pthread_mutex_init(&r->sim_lock, NULL);
    r->forks  = malloc(sizeof(pthread_mutex_t) * r->philo_nbr);
    r->philos = malloc(sizeof(t_philo) * r->philo_nbr);
    if (!r->forks || !r->philos)
        return (false);
    i = -1;
    while (++i < r->philo_nbr)
        pthread_mutex_init(&r->forks[i].fork, NULL);
    i = -1;
    while (++i < r->philo_nbr)
        set_philo(r, i);
    if (!create_threads(r))
        return (false);
    return (true);
}
