/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readd_write_stop.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karlarod <karlarod@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 18:28:50 by karlarod          #+#    #+#             */
/*   Updated: 2025/12/15 19:02:47 by karlarod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	print_status(t_philosophers *philo, e_philo_status state)
{
	struct timeval	current;
	double			time_elapsed;
	struct timeval  reference;
	int				time_stamp;
	
	if (philo->status->stop_simulation)
		return ;
	reference = philo->conditions->start;
	pthread_mutex_lock(philo->status->print);
	gettimeofday(&current, NULL);
	time_elapsed = (current.tv_sec - reference.tv_sec) * 1e6;
	time_elapsed = (time_elapsed + (current.tv_usec - reference.tv_usec)) * 1e-6;
	time_stamp = (time_elapsed / 1000);
	if (state == FORK)
		printf("%i %i has taken a fork\n",time_stamp, philo->index);
	else if (state == EAT)
		printf("%i %i is eating\n",time_stamp, philo->index);
	else if (state == SLEEP)
		printf("%i %i is sleeping\n",time_stamp, philo->index);
	else if (state == THINK)
		printf("%i %i is thinking\n",time_stamp, philo->index);
	else if (state == DEAD)
		printf("%i %i died\n",time_stamp, philo->index);
	pthread_mutex_lock(philo->status->print);
}

bool	read_stop_simulation(t_status *status)
{
	bool	value;

	pthread_mutex_lock(status->stop);
	value = status->stop_simulation;
	pthread_mutex_unlock(status->stop);
	return (value);
}

void	write_stop_simulation(t_philosophers *philo)
{
	pthread_mutex_lock(philo->status->stop);
	print_status(philo, DEAD);
	philo->status->stop_simulation = true;
	pthread_mutex_unlock(philo->status->stop);
}

