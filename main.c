/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karlarod <karlarod@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 16:04:56 by karlarod          #+#    #+#             */
/*   Updated: 2025/12/16 17:58:02 by karlarod         ###   ########.fr       */
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

void	destroy_mutex_forks(int n_philo, pthread_mutex_t *fork, t_status *status)
{
	int	i;

	i = 0;
	while (i < n_philo)
	{
		pthread_mutex_destroy(&fork[i]);
		pthread_mutex_destoy(&status->forks_used[i], NULL);
		++i;
	}
	pthread_mutex_destroy(status->print);
	pthread_mutex_destroy(status->stop);
	free(fork);
	free(status->print);
	free(status->stop);
	//pthread_mutex_destroy(&table->read);
}
void	*destroy_status(t_status *statua, int n_philo)
{
	int				i;
	
	i = 0;
	pthread_mutex_init(status->print, NULL);
	pthread_mutex_init(status->stop, NULL);
	while (i < n_philo)
	{
		pthread_mutex_destroy(&status->forks_used[i]);
		++i;
	}
	free(status->print);
	free(status->stop);
	free(status->forks_used); 
	free(status->t_last_meal);
	free(status->mutx_last_meal);
	free(status);
}

handle_mutex_init_fail(status->forks_used, i)

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
			return (handle_mutex_init_fail(status->forks_used, i);
		++i;
	}
	if (pthread_mutex_init(&status->print, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&status->stop, NULL) != 0)
		return(1);
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
	if (init_status(status, n_philo) !
	return (status);
}

void	init_philo(int n_philo, t_parameters *input)
{
	int				i;
	pthread_t		*philos;
	pthread_t		monitor_id;
	pthread_mutex_t	*forks;
	t_philosophers	*table;
	t_status		*status;
	int				ret;

	i = 0;
	philos = malloc(n_philo * sizeof(pthread_t ));
	table = malloc(n_philo * sizeof(t_philosophers ));
	
	status = init_status(n_philo, forks);
	while (i < n_philo)
	{
		table[i].conditions = input;
		table[i].index = i;
		table[i].l_fork = &forks[(i + 1) % n_philo];
		table[i].r_fork = &forks[i];
		table[i].status = status;
		//printf("create thread philo %i\n", i);
		ret = pthread_create(&philos[i], NULL, philos_routine, (void *)&table[i]);
		if (ret != 0)
		{
   			 //printf("pthread_create failed at %d, ret=%d\n", i, ret);
    		return;
		}
		++i;
	}
	while (i > 0)
	{
		--i;
		pthread_join(philos[i], NULL);
	}
	free(philos);
	destroy_mutex_forks(n_philo, forks, status);
	//free(status->forks_used);
	free(status);
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
		n_philo = ft_atoi(argv[1]);
		initial_conditions.time_die = ft_atoi(argv[2]) * 1000;
		initial_conditions.time_eat = ft_atoi(argv[3]) * 1000;
		initial_conditions.time_sleep = ft_atoi(argv[4]) * 1000;
		printf("Initial conditions %f %f %f \n", initial_conditions.time_die, initial_conditions.time_eat, initial_conditions.time_sleep );
		if (argc == 6)
			initial_conditions.n_dinners = ft_atoi(argv[5]);
		else
			initial_conditions.n_dinners = -1;
		init_philo(n_philo, &initial_conditions);
	}
	return (0);
}
