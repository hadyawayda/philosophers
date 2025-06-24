/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 18:38:45 by hawayda           #+#    #+#             */
/*   Updated: 2025/06/24 22:07:47 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*safe_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
		error_exit("Error: malloc failed\n");
	return (ptr);
}

static void	handle_mutex_error(t_opcode opcode, int status)
{
	if (0 == status)
		return ;
	if (EINVAL == status && (LOCK == opcode || UNLOCK == opcode))
		error_exit("Invalid mutex operation code.\n");
	else if (EINVAL == status && INIT == opcode)
		error_exit("The value specified by mutex is invalid.\n");
	else if (EDEADLK == status)
		error_exit("A deadlock would occur if the thread blocked \
			waiting for mutex.\n");
	else if (EPERM == status)
		error_exit("The current thread does not hold a lock on mutex.\n");
	else if (ENOMEM == status)
		error_exit("The process cannot allocate enough memory to \
			create another mutex.\n");
	else if (EBUSY == status)
		error_exit("Mutex is locked.\n");
}

void	safe_mutex_handler(t_mutex *mutex, t_opcode opcode)
{
	if (LOCK == opcode)
		handle_mutex_error(pthread_mutex_lock(mutex), opcode);
	else if (UNLOCK == opcode)
		handle_mutex_error(pthread_mutex_unlock(mutex), opcode);
	else if (INIT == opcode)
		handle_mutex_error(pthread_mutex_init(mutex, NULL), opcode);
	else if (DESTROY == opcode)
		handle_mutex_error(pthread_mutex_destroy(mutex), opcode);
	else
		error_exit("Invalid mutex operation code.\n");
}

static void	handle_thread_error(int status, t_opcode opcode)
{
	if (0 == status)
		return ;
	if (EAGAIN == status)
		error_exit("The thread resource limit has been reached.\n");
	else if (EPERM == status)
		error_exit("The current thread does not hold a lock on mutex.\n");
	else if (EINVAL == status && CREATE == opcode)
		error_exit("Invalid thread operation code.\n");
	else if (EINVAL == status && (JOIN == opcode || DETACH == opcode))
		error_exit("The value specified by thread is invalid.\n");
	else if (ESRCH == status)
		error_exit("The thread does not exist.\n");
	else if (EDEADLK == status)
		error_exit("A deadlock would occur if the thread blocked waiting \
			for another thread.\n");
}

void	safe_thread_handler(pthread_t *thread, void *(*f)(void *), void *data,
		t_opcode opcode)
{
	if (CREATE == opcode)
		handle_thread_error(pthread_create(thread, NULL, f, data), opcode);
	else if (JOIN == opcode)
		handle_thread_error(pthread_join(*thread, NULL), opcode);
	else if (DETACH == opcode)
		handle_thread_error(pthread_detach(*thread), opcode);
	else
		error_exit("Invalid thread operation code.\n");
}
