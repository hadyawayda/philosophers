/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 15:19:37 by hawayda           #+#    #+#             */
/*   Updated: 2025/06/17 15:19:37 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int ft_atoi_pos(const char *s, bool *ok)
{
    long res = 0;

    while (*s == ' ' || (*s >= 9 && *s <= 13))
        s++;
    if (*s == '+')
        s++;
    if (*s < '0' || *s > '9')
        return (*ok = false, 0);
    while (*s >= '0' && *s <= '9')
    {
        res = res * 10 + (*s - '0');
        if (res > INT_MAX)
            return (*ok = false, 0);
        s++;
    }
    if (*s != '\0')
        return (*ok = false, 0);
    return ((int)res);
}
