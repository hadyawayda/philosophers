/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 15:17:55 by hawayda           #+#    #+#             */
/*   Updated: 2025/06/17 15:17:55 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool valid_int(long v)
{
    return (v > 0 && v <= INT_MAX);
}

bool parse_args(int ac, char **av, t_table *r)
{
    bool ok = true;
    long tmp;

    if (ac != 5 && ac != 6)
        return (false);
    tmp = ft_atoi_pos(av[1], &ok);
    r->philo_nbr = (int)tmp;
    r->time_to_die   = (int)ft_atoi_pos(av[2], &ok);
    r->time_to_eat   = (int)ft_atoi_pos(av[3], &ok);
    r->time_to_sleep = (int)ft_atoi_pos(av[4], &ok);
    r->must_eat = -1;
    if (ac == 6)
        r->must_eat = (int)ft_atoi_pos(av[5], &ok);
    if (!ok || !valid_int(r->philo_nbr)
        || !valid_int(r->time_to_die) || !valid_int(r->time_to_eat)
        || !valid_int(r->time_to_sleep)
        || (ac == 6 && !valid_int(r->must_eat)))
        return (false);
    return (true);
}

int one_philo_case(t_table *r)
{
    long ts = 0;

    printf("%ld %d has taken a fork\n", ts, 1);
    ft_usleep(r->time_to_die);
    ts = r->time_to_die;
    printf("%ld %d died\n", ts, 1);
    return (0);
}

void wait_end(t_table *r)
{
    int i;

    monitor(r);
    i = -1;
    while (++i < r->philo_nbr)
        pthread_join(r->philos[i].thread_id, NULL);
}

int main(int ac, char **av)
{
    t_table r;

    if (!parse_args(ac, av, &r))
        return (printf("invalid argument\n"), 1);
    if (r.philo_nbr == 1)
        return (one_philo_case(&r));
    if (!init_sim(&r))
        return (printf("Error: init failed\n"), 1);
    wait_end(&r);
    free_all(&r);
    return (0);
}
