/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karlarod <karlarod@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 16:04:56 by karlarod          #+#    #+#             */
/*   Updated: 2026/01/08 16:44:35 by karlarod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "philosophers.h"

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
}

void	init_threads(t_philosophers *table, t_monitor *monitor)
{
	int				i;
	pthread_t		philos[PHILO_MAX];
	pthread_t		monitor_id;

	i = 0;
	pthread_create(&monitor_id, NULL, monitorig_routine, monitor);
	while (i < monitor->param->n_philo)
	{
		if (pthread_create(&philos[i], NULL, philos_routine, &table[i]) != 0)
		{
			printf("Pthread_create failed at %d\n", i);
			break ;
		}
		++i;
	}
	exit_simulation(i, philos, &monitor_id);
}

void	init_simulation(t_parameters *input)
{
	t_monitor		monitor;
	t_philosophers	*table;
	pthread_mutex_t	*forks;

	forks = make_mutex_forks(input->n_philo);
	if (forks == NULL)
		return ;
	table = make_table(input, forks);
	if (table == NULL)
		return ;
	monitor.status = table[0].status;
	monitor.param = input;
	init_threads(table, &monitor);
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
			init_simulation(&input);
		else
		{
			printf("The program only accept positive integers and");
			printf(" a maximum of 200 Philosophers\n");
		}
	}
	return (0);
}
