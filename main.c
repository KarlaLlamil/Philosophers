/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karlarod <karlarod@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 16:04:56 by karlarod          #+#    #+#             */
/*   Updated: 2025/12/15 20:30:39 by karlarod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "philosophers.h"

void	init_mutex_forks(pthread_mutex_t *fork, t_status *status)
{
	int	i;

	i = 0;
	status->stop_simulation = false;
	while (i < 5)
	{
		pthread_mutex_init(&fork[i], NULL);
		++i;
	}
	pthread_mutex_init(status->print, NULL);
	pthread_mutex_init(status->stop, NULL);
}

void	destroy_mutex_forks(pthread_mutex_t *fork, t_status *status)
{
	int	i;

	i = 0;
	while (i < 5)
	{
		pthread_mutex_destroy(&fork[i]);
		++i;
	}
	pthread_mutex_destroy(status->print);
	pthread_mutex_destroy(status->stop);
	//pthread_mutex_destroy(&table->read);
}

void	init_philo(int n_philo, t_parameters *input)
{
	int				i;
	pthread_t		*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t print;
	pthread_mutex_t stop;
	t_philosophers	**table;
	t_status		status;

	i = 0;
	philos = malloc(n_philo * sizeof(pthread_t));
	table = malloc(n_philo * sizeof(t_philosophers));
	forks = malloc(n_philo * sizeof(pthread_mutex_t));
	status.print = &print;
	status.stop = &stop;
	status.stop_simulation = false;
	init_mutex_forks(forks, &status);
	gettimeofday(&(input->start), NULL);
	while (i < n_philo)
	{
		table[i] = malloc(sizeof(t_philosophers));
		table[i]->conditions = input;
		table[i]->index = i;
		table[i]->l_fork = &forks[(i + 1) % n_philo];
		table[i]->r_fork = &forks[i];
		table[i]->status = &status;
		pthread_create(&philos[i], NULL, philos_routine, (void *)table[i]);
		++i;
	}
	while (i > 0)
	{
		--i;
		pthread_join(philos[i], NULL);
	}
	destroy_mutex_forks(forks, &status);
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
		if (argc == 6)
			initial_conditions.n_dinners = ft_atoi(argv[5]);
		else
			initial_conditions.n_dinners = -1;
		init_philo(n_philo, &initial_conditions);
	}
	return (0);
}
