/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karlarod <karlarod@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 16:04:56 by karlarod          #+#    #+#             */
/*   Updated: 2026/01/07 16:22:58 by karlarod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "philosophers.h"

pthread_mutex_t	*make_mutex_forks(int n_philo)
{
	int				i;
	pthread_mutex_t	*fork;

	i = 0;
	fork = malloc(n_philo * sizeof(pthread_mutex_t ));
	if (fork == NULL)
		return (NULL);
	while (i < n_philo)
	{
		pthread_mutex_init(&fork[i], NULL);
		++i;
	}
	return (fork);
}

void	destroy_mutex_forks(int n_philo, pthread_mutex_t *fork)
{
	int	i;

	i = 0;
	if (fork == NULL)
		return ;
	while (i < n_philo)
	{
		pthread_mutex_destroy(&fork[i]);
		++i;
	}
	free(fork);
	//pthread_mutex_destroy(&table->read);
}
void	*destroy_status(t_status *status, int n_philo, int type)
{
	int				i;
	
	i = 0;
	if (status == NULL)
		return(NULL) ;
	if (type == 1)
	{
		pthread_mutex_destroy(&status->print);
		pthread_mutex_destroy(&status->stop);
		while (i < n_philo)
		{
			pthread_mutex_destroy(&status->mutx_last_meal[i]);
			++i;
		}
	}
	free(status->forks); 
	free(status->t_last_meal);
	free(status->mutx_last_meal);
	free(status);
	return (NULL);
}

int	handle_mutex_init_fail(pthread_mutex_t *mtx, int n)
{
	int	i;

	i = 0;
	if (n == 0)
		pthread_mutex_destroy(mtx);
	else
	{
		while(i < n)
		{
			pthread_mutex_destroy(&mtx[i]);
			++i;
		}
	}
	return (1);
}

int		init_status(t_status *status, int n_philo)
{
	struct timeval	start;
	int				i;
	
	i = 0;
	status->stop_simulation = false;
	gettimeofday(&start, NULL);
	status->start = start;
	while (i < n_philo)
	{
		status->forks[i] = -1;
		status->t_last_meal[i] = start;
		status->n_meals[i] = 0;
		if (pthread_mutex_init(&status->mutx_last_meal[i], NULL) != 0)
			return (handle_mutex_init_fail(status->mutx_last_meal, i));
		++i;
	}
	if (pthread_mutex_init(&status->print, NULL) != 0)
		return (handle_mutex_init_fail(&status->print, 0));
	if (pthread_mutex_init(&status->stop, NULL) != 0)
		return(handle_mutex_init_fail(&status->stop, 0));
	return (0);
}


t_status	*make_status(int n_philo)
{
	t_status		*status;

	status = malloc(sizeof(t_status));
	if (status == NULL)
		return (NULL);
	status->forks = malloc(n_philo * (sizeof(int)));
	if (status->forks == NULL)
		return (free(status), NULL);
	status->t_last_meal = malloc(n_philo * (sizeof(struct timeval)));
	if (status->t_last_meal == NULL)
		return (free(status->forks), free(status), NULL);
	status->mutx_last_meal = malloc(n_philo * (sizeof(pthread_mutex_t)));
	if (status->mutx_last_meal == NULL)
		return (free(status->t_last_meal) ,free(status->forks), free(status),NULL);
	status->n_meals = malloc(n_philo * sizeof(int));
	if (init_status(status, n_philo) == 1)
		return (destroy_status(status, n_philo, 1));
	return (status);
}

t_philosophers	*make_table(t_parameters *input, pthread_mutex_t *forks)
{
	int				i;
	t_status		*status;
	t_philosophers	*table;

	i = 0;
	status = make_status(input->n_philo);
	table = malloc(input->n_philo * sizeof(t_philosophers));
	if (table == NULL || forks == NULL || status == NULL)
	{
		destroy_status(status, input->n_philo, 0);
		destroy_mutex_forks(input->n_philo, forks);
		return (NULL);
	}
	while (i < input->n_philo)
	{
		table[i].param = input;
		table[i].i = i;
		table[i].l_fork = &forks[(i + 1) % input->n_philo];
		table[i].r_fork = &forks[i];
		table[i].status = status;
		++i;
	}
	return (table);
}

void	exit_simulation(int n, pthread_t *philos, pthread_t *monitor_id)
{
	int	i;

	i = n;
	while (i > 0)
	{
		--i;
		pthread_join(philos[i], NULL);
	}
	pthread_join(*monitor_id, NULL);
	free(philos);
}

void	init_philo(t_parameters *input)
{
	int				i;
	pthread_t		*philos;
	pthread_t		monitor_id;
	t_monitor		monitor;
	t_philosophers	*table;
	pthread_mutex_t	*forks;

	i = 0;
	forks = make_mutex_forks(input->n_philo);;
	philos = malloc(input->n_philo * sizeof(pthread_t ));
	table = make_table(input, forks);
	monitor.status = table[0].status;
	monitor.param = input;
	pthread_create(&monitor_id, NULL, monitorig_routine, (void *)&monitor);
	while (i < input->n_philo)
	{
		if (pthread_create(&philos[i], NULL, philos_routine, (void *)&table[i]) != 0)
		{
   			printf("pthread_create failed at %d\n", i);
    		break;
		}
		++i;
	}
	exit_simulation(monitor.param->n_philo, philos, &monitor_id);
	destroy_status(table[0].status, monitor.param->n_philo, 0);
	destroy_mutex_forks(monitor.param->n_philo, forks);
	free(table);
}


int	main(int argc, char **argv)
{
	t_parameters	input;
	bool			valid;
	
	input = (t_parameters){};
	if (argc < 5 || argc > 6)
	{
		printf("The program needs:\n1.Number of philosophers\n2.Time to die\n");
		printf("3.Time to eat\n4.Time to sleep\n");
		printf("5.Number of times each philosopher should eat(optional)\n");
	}
	else
	{
		valid = validate_input(argc, argv, &input);
		if (valid)
		{
			init_philo(&input);
		}
		else
			printf("Invalid input, the program only accept integers\n");
	}
	return (0);
}
