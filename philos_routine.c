/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_routine.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karlarod <karlarod@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 17:29:19 by karlarod          #+#    #+#             */
/*   Updated: 2026/01/08 15:03:29 by karlarod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>

void	think(t_philosophers *philo)
{
	print_status(philo->i, philo->status, THINK);
}

void	get_forks(t_philosophers *philo)
{
	int left;

	left = philo->i + 1;
	if ((philo->i + 1) == philo->param->n_philo)
		left = 0;
	if (read_stop_simulation(philo->status))
		return ;
	// while (philo->param->n_philo % 2 ==  1 && (philo->status->forks[philo->i] == philo->i || philo->status->forks[left] == philo->i))
	// 	usleep(5000);
	if ((philo->i + 1) != philo->param->n_philo)
	{
		pthread_mutex_lock(philo->r_fork);
		philo->status->forks[philo->i] = philo->i;
		print_status(philo->i, philo->status, FORK);
		pthread_mutex_lock(philo->l_fork);
		philo->status->forks[(philo->i + 1) % philo->param->n_philo] = philo->i;
		print_status(philo->i, philo->status, FORK);
	}
	else
	{
		pthread_mutex_lock(philo->l_fork);
		philo->status->forks[(philo->i + 1) % philo->param->n_philo] = philo->i;
		print_status(philo->i, philo->status, FORK);
		pthread_mutex_lock(philo->r_fork);
		philo->status->forks[philo->i] = philo->i;
		print_status(philo->i, philo->status, FORK);
	}
}

void	put_forks( t_philosophers *philo)
{
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
}

void	eat(t_philosophers *philo)
{
	struct timeval	current;
	long int		t_elapsed;
	struct timeval	start_eating;

	if (read_stop_simulation(philo->status))
		return ;
	gettimeofday(&start_eating, NULL);
	pthread_mutex_lock(&philo->status->mutx_last_meal[philo->i]);
	philo->status->t_last_meal[philo->i] = start_eating;
	pthread_mutex_unlock(&philo->status->mutx_last_meal[philo->i]);
	print_status(philo->i, philo->status, EAT);
	while (!read_stop_simulation(philo->status))
	{
		gettimeofday(&current, NULL);
		t_elapsed = (current.tv_sec - start_eating.tv_sec) * 1e6;
		t_elapsed = (t_elapsed + (current.tv_usec - start_eating.tv_usec));
		if (t_elapsed > philo->param->time_eat)
			break;
		if ((t_elapsed + 5000) > philo->param->time_eat)
			usleep(philo->param->time_eat - t_elapsed);
		else
			usleep(5000);			
	}
	++(philo->status->n_meals[philo->i]);
}

void	f_sleep( t_philosophers *philo)
{
	struct timeval	start_sleeping;
	long int	t_elapsed;
	struct timeval	current;

	if (read_stop_simulation(philo->status))
		return ;
	print_status(philo->i, philo->status, SLEEP);
	gettimeofday(&start_sleeping, NULL);
	while (!read_stop_simulation(philo->status))
	{
		gettimeofday(&current, NULL);
		t_elapsed = (current.tv_sec - start_sleeping.tv_sec) * 1e6;
		t_elapsed = (t_elapsed + (current.tv_usec - start_sleeping.tv_usec));
		if (t_elapsed >= philo->param->time_sleep)
			return ;
		if ((t_elapsed + 5000) > philo->param->time_sleep)
			usleep(philo->param->time_sleep - t_elapsed);
		else
			usleep(5000);
	}
}

void	*philos_routine(void *philosopher)
{
	t_philosophers	*philo;

	philo = (t_philosophers *)philosopher;
	while(!read_stop_simulation(philo->status))
	{
		get_forks(philo);
		eat (philo);
		put_forks (philo);
		f_sleep (philo);
		think (philo);
	}
	return (NULL);
}
