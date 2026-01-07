/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readd_write_stop.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karlarod <karlarod@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 18:28:50 by karlarod          #+#    #+#             */
/*   Updated: 2025/12/17 19:14:00 by karlarod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <stdio.h>

void	print_status(int n, t_status *status, e_philo_status state)
{
	struct timeval	current;
	double			time_elapsed;
	struct timeval  reference;
	int				time_stamp;
	
	reference = status->start;
	gettimeofday(&current, NULL);
	time_elapsed = (current.tv_sec - reference.tv_sec) * 1e6;
	time_elapsed = (time_elapsed + (current.tv_usec - reference.tv_usec));
	time_stamp = (time_elapsed / 1000);
	if (state == FORK && !read_stop_simulation(status))
		printf("%i %i has taken a fork\n",time_stamp, n);
	else if (state == EAT)
		printf("%i %i is eating\n",time_stamp, n);
	else if (state == SLEEP)
		printf("%i %i is sleeping\n",time_stamp, n);
	else if (state == THINK)
		printf("%i %i is thinking\n",time_stamp, n);
	else if (state == DEAD)
		printf("%i %i died\n",time_stamp, n);
	// pthread_mutex_unlock(&status->print);
}

bool	read_stop_simulation(t_status *status)
{
	bool	value;

	pthread_mutex_lock(&status->stop);
	value = status->stop_simulation;
	pthread_mutex_unlock(&status->stop);
	return (value);
}

void	write_stop_simulation(int n, t_status *status)
{
	pthread_mutex_lock(&status->stop);
	print_status(n, status, DEAD);
	status->stop_simulation = true;
	pthread_mutex_unlock(&status->stop);
}

