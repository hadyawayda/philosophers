#include "philo_bonus.h"

/*
 * Prints a timestamped status line, protected by a semaphore.
 */
void print_status(t_table *table, int id, const char *msg)
{
	sem_wait(table->print);
	printf("%ld %d %s\n",
	       get_time_ms() - table->start_time,
	       id,
	       msg);
	sem_post(table->print);
}

/*
 * In each philosopher **process**, we spawn a **thread** that watches
 * for death.  If this thread detects time_to_die exceeded, it prints
 * "died" and exits the process with status=1.
 */
static void *monitor_routine(void *arg)
{
	t_philo *ph = arg;

	while (1)
	{
		if (get_time_ms() - ph->last_meal > ph->table->time_to_die)
		{
			print_status(ph->table, ph->id, "died");
			exit(1);
		}
		usleep(1000);
	}
	return (NULL);
}

/*
 * This function runs in each child process.
 * It loops: take two forks, eat, release forks, sleep, think.
 * If meals_limit >= 0, it exits(0) after eating enough times.
 */
void philosopher_process(t_table *table, int id)
{
	t_philo ph;

	ph.table       = table;
	ph.id          = id;
	ph.last_meal   = get_time_ms();
	ph.meals_eaten = 0;

	if (pthread_create(&ph.monitor, NULL, monitor_routine, &ph) != 0)
		error_out("pthread_create failed");
	pthread_detach(ph.monitor);

	while (table->meals_limit < 0
	   || ph.meals_eaten < table->meals_limit)
	{
		sem_wait(table->forks);
		print_status(table, id, "has taken a fork");
		sem_wait(table->forks);
		print_status(table, id, "has taken a fork");

		ph.last_meal = get_time_ms();
		print_status(table, id, "is eating");
		precise_usleep(table->time_to_eat);
		ph.meals_eaten++;

		sem_post(table->forks);
		sem_post(table->forks);

		print_status(table, id, "is sleeping");
		precise_usleep(table->time_to_sleep);

		print_status(table, id, "is thinking");
	}

	/* finished eating required times */
	exit(0);
}
