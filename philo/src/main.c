/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 03:04:57 by hawayda           #+#    #+#             */
/*   Updated: 2025/06/29 03:07:43 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	valid_int(long v)
{
	return (v > 0 && v <= INT_MAX);
}

bool	parse_args(int ac, char **av, t_rules *r)
{
	bool	ok;
	long	tmp;

	if (ac != 5 && ac != 6)
		return (false);
	ok = true;
	tmp = ft_atoi_pos(av[1], &ok);
	r->n_philo = (int)tmp;
	r->t_die = (int)ft_atoi_pos(av[2], &ok);
	r->t_eat = (int)ft_atoi_pos(av[3], &ok);
	r->t_sleep = (int)ft_atoi_pos(av[4], &ok);
	r->must_eat = -1;
	if (ac == 6)
		r->must_eat = (int)ft_atoi_pos(av[5], &ok);
	if (!ok || !valid_int(r->n_philo) || !valid_int(r->t_die)
		|| !valid_int(r->t_eat) || !valid_int(r->t_sleep) || (ac == 6
			&& !valid_int(r->must_eat)))
		return (false);
	return (true);
}

static int	one_philo_case(t_rules *r)
{
	long	ts;

	ts = 0;
	printf("%ld %d has taken a fork\n", ts, 1);
	ft_usleep(r->t_die);
	ts = r->t_die;
	printf("%ld %d died\n", ts, 1);
	return (0);
}

static void	wait_end(t_rules *r)
{
	int	i;

	monitor(r);
	i = -1;
	while (++i < r->n_philo)
		pthread_join(r->philos[i].thread, NULL);
}

int	main(int ac, char **av)
{
	t_rules	r;

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
