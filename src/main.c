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

int one_philo_case(t_rules *r)
{
    long ts = 0;

    printf("%ld %d has taken a fork\n", ts, 1);
    ft_usleep(r->t_die);
    ts = r->t_die;
    printf("%ld %d died\n", ts, 1);
    return (0);
}

void wait_end(t_rules *r)
{
    int i;

    monitor(r);
    i = -1;
    while (++i < r->n_philo)
        pthread_join(r->philos[i].thread, NULL);
}

int main(int ac, char **av)
{
    t_rules r;

    if (!parse_args(ac, av, &r))
        return (printf("invalid argument\n"), 1);
    if (r.n_philo == 1)
        return (one_philo_case(&r));
    if (!init_sim(&r))
        return (printf("Error: init failed\n"), 1);
    wait_end(&r);
    free_all(&r);
    return (0);
}
