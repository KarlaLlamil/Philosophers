/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_routine.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karlarod <karlarod@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 16:56:46 by karlarod          #+#    #+#             */
/*   Updated: 2026/01/08 17:09:12 by karlarod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>

void	think(t_philosophers *philo)
{
	if (read_stop_simulation(philo->status))
		return ;
	print_status(philo->index, philo->status, THINK);
}

void	get_forks(int left, t_philosophers *philo)
{
	if (left == philo->param->n_philo)
		left = 0;
	if (read_stop_simulation(philo->status))
		return ;
	while (philo->param->n_philo%2 ==  1 && (philo->status->forks_used[philo->index] == philo->index || philo->status->forks_used[left] == philo->index))
		usleep(5000);
	if (left != 0)
	{
		pthread_mutex_lock(philo->r_fork);
		philo->status->forks_used[philo->index] = philo->index;
		print_status(philo->index, philo->status, FORK);
		pthread_mutex_lock(philo->l_fork);
		philo->status->forks_used[left] = philo->index;
		print_status(philo->index, philo->status, FORK);
	}
	else
	{
		pthread_mutex_lock(philo->l_fork);
		philo->status->forks_used[left] = philo->index;
		print_status(philo->index, philo->status, FORK);
		pthread_mutex_lock(philo->r_fork);
		philo->status->forks_used[philo->index] = philo->index;
		print_status(philo->index, philo->status, FORK);
	}
}

void	put_forks( t_philosophers *philo)
{
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
}

void	eat(int  *n_meals, t_philosophers *philo)
{
	struct timeval	current;
	double			time_elapsed;
	struct timeval	start;

	if (read_stop_simulation(philo->status))
		return ;
	gettimeofday(&start, NULL);
	pthread_mutex_lock(&philo->status->mutx_last_meal[philo->index]);
	philo->status->t_last_meal[philo->index] = start;
	pthread_mutex_unlock(&philo->status->mutx_last_meal[philo->index]);
	print_status(philo->index, philo->status, EAT);
	while (!read_stop_simulation(philo->status))
	{
		gettimeofday(&current, NULL);
		time_elapsed = (current.tv_sec - start.tv_sec) * 1e6;
		time_elapsed = (time_elapsed + (current.tv_usec - start.tv_usec));
		if (time_elapsed > philo->param->time_eat)
			break ;
		if ((time_elapsed + 5000) > philo->param->time_eat)
			usleep(philo->param->time_eat - time_elapsed);
		else
			usleep(5000);
	}
	++(*n_meals);
	philo->status->n_meals[philo->index] = *n_meals;
}

void	f_sleep( t_philosophers *philo)
{
	struct timeval	start;
	double			time_elapsed;
	struct timeval	current;

	if (read_stop_simulation(philo->status))
		return ;
	print_status(philo->index, philo->status, SLEEP);
	gettimeofday(&start, NULL);
	while (!read_stop_simulation(philo->status))
	{
		gettimeofday(&current, NULL);
		time_elapsed = (current.tv_sec - start.tv_sec) * 1e6;
		time_elapsed = (time_elapsed + (current.tv_usec - start.tv_usec));
		if (time_elapsed >= philo->param->time_sleep)
			return ;
		if ((time_elapsed + 5000) > philo->param->time_sleep)
			usleep(philo->param->time_sleep - time_elapsed);
		else
			usleep(5000);
	}
}

void	*philos_routine(void *philosopher)
{
	int				n_meals;
	t_philosophers	*philo;

	n_meals = 0;
	philo = (t_philosophers *)philosopher;
	while (!read_stop_simulation(philo->status))
	{
		if (philo->param->n_dinners >= 0 && n_meals == philo->param->n_dinners)
			return (NULL);
		if (philo->l_fork != philo->r_fork)
		{
			get_forks(philo->index + 1, philo);
			eat(&n_meals, philo);
			put_forks (philo);
		}
		f_sleep (philo);
		think (philo);
	}
	return (NULL);
}
