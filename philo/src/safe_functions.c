/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42beirut.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 12:35:39 by hawayda           #+#    #+#             */
/*   Updated: 2025/07/24 12:35:39 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*safe_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
		error_out("Error: malloc failed\n");
	return (ptr);
}

static void	handle_mutex_error(t_opcode opcode, int status)
{
	if (0 == status)
		return ;
	if (EINVAL == status && (LOCK == opcode || UNLOCK == opcode))
		error_out("Invalid mutex operation code.\n");
	else if (EINVAL == status && INIT == opcode)
		error_out("The value specified by mutex is invalid.\n");
	else if (EDEADLK == status)
		error_out("A deadlock would occur if the thread blocked \
			waiting for mutex.\n");
	else if (EPERM == status)
		error_out("The current thread does not hold a lock on mutex.\n");
	else if (ENOMEM == status)
		error_out("The process cannot allocate enough memory to \
			create another mutex.\n");
	else if (EBUSY == status)
		error_out("Mutex is locked.\n");
}

void	safe_mutex_handler(t_mutex *mutex, t_opcode opcode)
{
	if (LOCK == opcode)
		handle_mutex_error(opcode, pthread_mutex_lock(mutex));
	else if (UNLOCK == opcode)
		handle_mutex_error(opcode, pthread_mutex_unlock(mutex));
	else if (INIT == opcode)
		handle_mutex_error(opcode, pthread_mutex_init(mutex, NULL));
	else if (DESTROY == opcode)
		handle_mutex_error(opcode, pthread_mutex_destroy(mutex));
	else
		error_out("Invalid mutex operation code.\n");
}

static void	handle_thread_error(int status, t_opcode opcode)
{
	if (0 == status)
		return ;
	if (EAGAIN == status)
		error_out("The thread resource limit has been reached.\n");
	else if (EPERM == status)
		error_out("The current thread does not hold a lock on mutex.\n");
	else if (EINVAL == status && CREATE == opcode)
		error_out("Invalid thread operation code.\n");
	else if (EINVAL == status && (JOIN == opcode || DETACH == opcode))
		error_out("The value specified by thread is invalid.\n");
	else if (ESRCH == status)
		error_out("The thread does not exist.\n");
	else if (EDEADLK == status)
		error_out("A deadlock would occur if the thread blocked waiting \
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
		error_out("Invalid thread operation code.\n");
}
