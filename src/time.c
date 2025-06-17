/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 15:19:04 by hawayda           #+#    #+#             */
/*   Updated: 2025/06/17 15:19:04 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long get_time_ms(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
}

void ft_usleep(long ms)
{
    long start = get_time_ms();

    while (get_time_ms() - start < ms)
        usleep(100);
}