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

int one_philo_case(t_table *table)
{
    long ts = 0;

    printf("%ld %d has taken a fork\n", ts, 1);
    ft_usleep(table->time_to_die);
    ts = table->time_to_die;
    printf("%ld %d died\n", ts, 1);
    return (0);
}

void wait_end(t_table *table)
{
    int i;

    monitor(table);
    i = -1;
    while (++i < table->philo_nbr)
        pthread_join(table->philos[i].thread_id, NULL);
}

int main(int ac, char **av)
{
    t_table table;

    if (ac != 5 && ac != 6)
        error_exit("Error: invalid arguments\n"GREEN"Correct usage: ./philo 5 800 200 200 [5]"RESET);
    parse_input(&table, av);
    if (table.philo_nbr == 1)
        return (one_philo_case(&table));
    if (!init_sim(&table))
        return (printf("Error: init failed\n"), 1);
    wait_end(&table);
    free_all(&table);
    return (0);
}
