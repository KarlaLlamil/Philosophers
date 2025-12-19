/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karlarod <karlarod@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 15:23:27 by karlarod          #+#    #+#             */
/*   Updated: 2025/12/17 19:11:31 by karlarod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>

void *monitorig_routine(void *args)
{
	struct timeval current;
	double			time_elapsed;
//	struct timeval	last;
	//double			timer;
	int				i;
	t_monitor		*monitor;

	i  = 0;
	monitor = (t_monitor *)args;
	while (!read_stop_simulation(monitor->status))
	{
		usleep(5000);
		i = 0;
		while (i < monitor->conditions->n_philos)
		{
			//printf("in the loop\n");
			gettimeofday(&current, NULL);
			pthread_mutex_lock(&monitor->status->mutx_last_meal[i]);
			time_elapsed = (current.tv_sec - monitor->status->t_last_meal[i].tv_sec) * 1e6;
			time_elapsed = (time_elapsed + (current.tv_usec - monitor->status->t_last_meal[i].tv_usec));
			pthread_mutex_unlock(&monitor->status->mutx_last_meal[i]);
			//printf("time elapsed %f\n", time_elapsed);
			if (time_elapsed > monitor->conditions->time_die)
			{
				write_stop_simulation(i, monitor->status);
				return (NULL);
				//break;
			}
			++i;
		}
	}
	return (NULL);
}