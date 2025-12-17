/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karlarod <karlarod@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 16:04:56 by karlarod          #+#    #+#             */
/*   Updated: 2025/12/17 15:17:05 by karlarod         ###   ########.fr       */
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
		return ;
	if (type == 1)
	{
		pthread_mutex_destroy(&status->print);
		pthread_mutex_destroy(&status->stop);
		while (i < n_philo)
		{
			pthread_mutex_destroy(&status->forks_used[i]);
			++i;
		}
	}
	free(status->forks_used); 
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
	status->starving_p = 0;
	status->stop_simulation = false;
	gettimeofday(&start, NULL);
	while (i < n_philo)
	{
		status->forks_used[i] = -1;
		status->t_last_meal[i] = start;
		if (pthread_mutex_init(&status->forks_used[i], NULL) != 0)
			return (handle_mutex_init_fail(status->forks_used, i));
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
	status->forks_used = malloc(n_philo * (sizeof(int)));
	if (status->forks_used == NULL)
		return (free(status), NULL);
	status->t_last_meal = malloc(n_philo * (sizeof(struct timeval)));
	if (status->t_last_meal == NULL)
		return (free(status->forks_used), free(status), NULL);
	status->mutx_last_meal = malloc(n_philo * (sizeof(pthread_mutex_t)));
	if (status->mutx_last_meal == NULL)
		return (free(status->t_last_meal) ,free(status->forks_used), free(status),NULL);
	if (init_status(status, n_philo) == 1)
		return (destroy_status(status, n_philo, 1));
	return (status);
}

t_philosophers	*make_table(t_parameters *input, pthread_mutex_t **forks)
{
	int				i;
	t_status		*status;
	t_philosophers	*table;

	i = 0;
	status = make_status(input->n_philos);
	*forks = make_mutex_forks(input->n_philos);
	table = malloc(input->n_philos * sizeof(t_philosophers));
	if (table == NULL || *forks == NULL || status == NULL)
	{
		destroy_status(status, input->n_philos, 0);
		destroy_mutex_forks(input->n_philos, *forks);
		return (NULL);
	}
	while (i < input->n_philos)
	{
		table[i].conditions = input;
		table[i].index = i;
		table[i].l_fork = &forks[(i + 1) % input->n_philos];
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
	forks = NULL;
	philos = malloc(input->n_philos * sizeof(pthread_t ));
	table = make_table(input, &forks);
	monitor.status = table[0].status;
	monitor.conditions = input;
	pthread_create(&monitor_id, NULL, monitorig_routine, (void *)&monitor);
	while (i < input->n_philos)
	{
		//printf("create thread philo %i\n", i);
		if (pthread_create(&philos[i], NULL, philos_routine, (void *)&table[i]) != 0)
		{
   			printf("pthread_create failed at %d, ret=%d\n", i);
    		return;
		}
		++i;
	}
	exit_simulation(n_philo, philos, &monitor_id);
	destroy_status(table[0].status, n_philo, 0);
	destroy_mutex_forks(n_philo, forks);
	free(table);
}


int	main(int argc, char **argv)
{
	t_parameters	initial_conditions;
	int				n_philo;
	
	initial_conditions = (t_parameters){};
	if (argc < 5 || argc > 6)
		printf("The program needs number of philosophers time ...");
	else if (validate_input(argc, argv) == false)
		printf("The input needs to be an int ");
	else
	{
		initial_conditions.n_philos = ft_atoi(argv[1]);
		initial_conditions.time_die = ft_atoi(argv[2]) * 1000;
		initial_conditions.time_eat = ft_atoi(argv[3]) * 1000;
		initial_conditions.time_sleep = ft_atoi(argv[4]) * 1000;
		printf("Initial conditions %f %f %f \n", initial_conditions.time_die, initial_conditions.time_eat, initial_conditions.time_sleep );
		if (argc == 6)
			initial_conditions.n_dinners = ft_atoi(argv[5]);
		else
			initial_conditions.n_dinners = -1;
		init_philo(&initial_conditions);
	}
	return (0);
}
