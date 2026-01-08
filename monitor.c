/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karlarod <karlarod@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 15:23:27 by karlarod          #+#    #+#             */
/*   Updated: 2026/01/08 16:53:05 by karlarod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>

bool	finish_dinner(t_monitor *monitor)
{
	int	i;

	i = 0;
	while (i < monitor->param->n_philo)
	{
		if (monitor->status->n_meals[i] < monitor->param->n_dinners)
			return (false);
		++i;
	}
	return (true);
}

double	calculate_time_elapsed(t_monitor *monitor, int i)
{
	struct timeval	current;
	long int		time_elapsed;
	struct timeval	last;

	gettimeofday(&current, NULL);
	pthread_mutex_lock(&monitor->status->mutx_last_meal[i]);
	last = monitor->status->t_last_meal[i];
	pthread_mutex_unlock(&monitor->status->mutx_last_meal[i]);
	time_elapsed = (current.tv_sec - last.tv_sec) * 1e6;
	time_elapsed = (time_elapsed + (current.tv_usec - last.tv_usec));
	return (time_elapsed);
}

void	*monitorig_routine(void *args)
{
	int				i;
	t_monitor		*monitor;

	i = 0;
	monitor = (t_monitor *)args;
	while (!read_stop_simulation(monitor->status))
	{
		if (monitor->param->n_dinners != -1 && finish_dinner(monitor))
		{
			write_stop_simulation(-1, monitor->status);
			return (NULL);
		}
		usleep(5000);
		i = 0;
		while (i < monitor->param->n_philo)
		{
			if (calculate_time_elapsed(monitor, i) > monitor->param->time_die)
			{
				write_stop_simulation(i, monitor->status);
				return (NULL);
			}
			++i;
		}
	}
	return (NULL);
}
