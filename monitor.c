/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karlarod <karlarod@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 15:23:27 by karlarod          #+#    #+#             */
/*   Updated: 2025/12/17 15:25:51 by karlarod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void *monitorig_routine(void *args)
{
	struct timeval current;
	double			time_elapsed;
	int				i;
	t_monitor		*monitor;

	i  = 0;
	monitor = (t_monitor *)args;
	while (i < monitor->conditions->n_philos)
	{
		pthread_mutex_lock(&monitor->status->mutx_last_meal[i]);
		time_elapsed = (current.tv_sec - monitor->status->t_last_meal[i].tv_sec) * 1e6;
		time_elapsed = (time_elapsed + (current.tv_usec - monitor->status->t_last_meal[i].tv_usec));
		
		
	}
}