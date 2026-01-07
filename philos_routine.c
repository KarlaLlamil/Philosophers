/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_routine.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karlarod <karlarod@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 17:29:19 by karlarod          #+#    #+#             */
/*   Updated: 2026/01/07 17:29:20 by karlarod         ###   ########.fr       */
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

int	get_forks(t_philosophers *philo)
{
	if (read_stop_simulation(philo->status))
		return (0) ;
	while (philo->conditions->n_philos%2 ==  1 && (philo->status->forks_used[philo->index] == philo->index || philo->status->forks_used[(philo->index + 1) % philo->conditions->n_philos] == philo->index))
	{
		usleep(5000);
	}
	if (philo->index < (philo->index + 1) % philo->conditions->n_philos)
	{

		pthread_mutex_lock(philo->r_fork);
		philo->status->forks_used[philo->index] = philo->index;
		//philo->status->forks_used[philo->index] = philo->index;
		// printf("philosopher %i take fork %i\n", philo->index, philo->index);
		//if (read_stop_simulation(philo->status))
		//	return (1);
		print_status(philo->index, philo->status, FORK_R);
	
		//philo->status->forks_used[philo->index] = philo->index;
		pthread_mutex_lock(philo->l_fork);
		philo->status->forks_used[(philo->index + 1) % philo->conditions->n_philos] = philo->index;
		//printf("philosopher %i take fork %i\n", philo->index, (philo->index + 1) % philo->conditions->n_philos);
		//if (read_stop_simulation(philo->status))
		//	return (2);
		print_status(philo->index, philo->status, FORK_L);
		//philo->status->forks_used[(philo->index + 1) % n_philo] = philo->index;
	}
	else
	{
		// while (philo->status->forks_used[(philo->index + 1) % philo->conditions->n_philos] == philo->index)
		// {
		// 	usleep(5000);
		// }
		pthread_mutex_lock(philo->l_fork);
		philo->status->forks_used[(philo->index + 1) % philo->conditions->n_philos] = philo->index;
		//printf("philosopher %i take fork %i\n", philo->index, (philo->index + 1) % philo->conditions->n_philos);
		//if (read_stop_simulation(philo->status) )
		//	return (1);
		print_status(philo->index, philo->status, FORK_L);
		//if (philo->l_fork == philo->r_fork)
		//	return(1);
		pthread_mutex_lock(philo->r_fork);
		philo->status->forks_used[philo->index] = philo->index;
		//printf("philosopher %i take fork %i\n", philo->index, philo->index);
		//if (read_stop_simulation(philo->status))
		//	return (2);
		print_status(philo->index, philo->status, FORK_R);
		//philo->status->forks_used[philo->index] = philo->index;
	}
	return (2);
	// pthread_mutex_lock(&philo->print);
	// printf("forks picked up right-> %i left->%i\n",philo->index, left);
	// pthread_mutex_unlock(&philo->print);
}

void	put_forks( t_philosophers *philo)
{
	//printf("put forks philo %i\n", ((t_philosophers *)philo)->index);
	// if (philo->index%2 == 0 && n_mut != 0)
	// {
	pthread_mutex_unlock(philo->r_fork);
	//if (n_mut == 2)
	pthread_mutex_unlock(philo->l_fork);
	// }
	// else if (n_mut != 0)
	// {
	// 	pthread_mutex_unlock(philo->l_fork);
	// 	if (n_mut == 2)
	// 		pthread_mutex_unlock(philo->r_fork);
	// }
}

void	eat( int  *n_meals, t_philosophers *philo)
{
	struct timeval current;
	double			time_elapsed;
	struct timeval	start_eating;

	if (read_stop_simulation(philo->status))
		return ;
	gettimeofday(&start_eating, NULL);
	pthread_mutex_lock(&philo->status->mutx_last_meal[philo->index]);
	philo->status->t_last_meal[philo->index] = start_eating;
	pthread_mutex_unlock(&philo->status->mutx_last_meal[philo->index]);
	print_status(philo->index, philo->status, EAT);
	while (!read_stop_simulation(philo->status))
	{
		gettimeofday(&current, NULL);
		time_elapsed = (current.tv_sec - start_eating.tv_sec) * 1e6;
		time_elapsed = (time_elapsed + (current.tv_usec - start_eating.tv_usec));
		if (time_elapsed > philo->conditions->time_eat)
			break;
		if ((time_elapsed + 5000) > philo->conditions->time_eat)
			usleep(philo->conditions->time_eat - time_elapsed);
		else
			usleep(5000);			
	}
	++(*n_meals);
}

void	f_sleep( t_philosophers *philo)
{
	struct timeval	start_sleeping;
	double			time_elapsed;
	struct timeval	current;

	if (read_stop_simulation(philo->status))
		return ;
	print_status(philo->index, philo->status, SLEEP);
	gettimeofday(&start_sleeping, NULL);
	while (!read_stop_simulation(philo->status))
	{
		gettimeofday(&current, NULL);
		time_elapsed = (current.tv_sec - start_sleeping.tv_sec) * 1e6;
		time_elapsed = (time_elapsed + (current.tv_usec - start_sleeping.tv_usec));
		if (time_elapsed >= philo->conditions->time_sleep)
			return ;
		if ((time_elapsed + 5000) > philo->conditions->time_sleep)
			usleep(philo->conditions->time_sleep - time_elapsed);
		else
			usleep(5000);
	}
}

void	*philos_routine(void *philo)
{
	int				n_meals;

	n_meals = 0;
	while(!read_stop_simulation(((t_philosophers *)philo)->status))
	{
		if (((t_philosophers *)philo)->conditions->n_dinners >= 0 && n_meals == ((t_philosophers *)philo)->conditions->n_dinners)
			return (NULL);
		// Revisar si considerar que tome un tenedor
		if (((t_philosophers *)philo)->l_fork != ((t_philosophers *)philo)->r_fork)
		{
			get_forks((t_philosophers *)philo);
		//if (n_mtx == 2)
			eat (&n_meals, (t_philosophers*)philo);
			put_forks ( (t_philosophers*)philo);
		}
		f_sleep ( (t_philosophers *)philo);
		think ( (t_philosophers *)philo);
	}
	return (NULL);
	
}