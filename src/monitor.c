/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 15:18:49 by hawayda           #+#    #+#             */
/*   Updated: 2025/06/24 22:02:37 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// void log_state(t_philo *p, const char *msg, bool death)
// {
//     long ts;

//     pthread_mutex_lock(&p->table->print);
//     pthread_mutex_lock(&p->table->sim_lock);
//     if (!p->table->end_simulation || death)
//     {
//         ts = get_time_ms() - p->table->start_simulation;
//         printf("%ld %d %s\n", ts, p->id, msg);
//     }
//     pthread_mutex_unlock(&p->table->sim_lock);
//     pthread_mutex_unlock(&p->table->print);
// }

// static bool all_fed(t_table *table)
// {
//     int i = 0;
//     int full = 0;

//     if (table->must_eat < 0)
//         return (false);
//     while (i < table->philo_nbr)
//     {
//         pthread_mutex_lock(&table->philos[i].lock);
//         if (table->philos[i].meals_counter >= table->must_eat)
//             full++;
//         pthread_mutex_unlock(&table->philos[i].lock);
//         i++;
//     }
//     return (full == table->philo_nbr);
// }

// static bool check_death(t_table *table, int i)
// {
//     long now;
//     bool first;

//     pthread_mutex_lock(&table->philos[i].lock);
//     now = get_time_ms();
//     if (now - table->philos[i].last_meal_time >= table->time_to_die)
//     {
//         pthread_mutex_unlock(&table->philos[i].lock);
//         pthread_mutex_lock(&table->sim_lock);
//         first = !table->end_simulation;
//         table->end_simulation = true;
//         pthread_mutex_unlock(&table->sim_lock);
//         if (first)
//             log_state(&table->philos[i], "died", true);
//         exit(0);
//     }
//     pthread_mutex_unlock(&table->philos[i].lock);
//     return (false);
// }

// void monitor(t_table *table)
// {
//     int i;

//     while (true)
//     {
//         pthread_mutex_lock(&table->sim_lock);
//         if (table->end_simulation)
//             exit(0);
//         pthread_mutex_unlock(&table->sim_lock);
//         i = 0;
//         while (i < table->philo_nbr)
//             check_death(table, i++);
//         if (all_fed(table))
//         {
//             pthread_mutex_lock(&table->sim_lock);
//             table->end_simulation = true;
//             pthread_mutex_unlock(&table->sim_lock);
//             exit(0);
//         }
//         usleep(100);
//     }
// }
