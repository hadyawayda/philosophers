/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 15:19:16 by hawayda           #+#    #+#             */
/*   Updated: 2025/06/17 15:19:16 by hawayda          ###   ########.fr       */
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
