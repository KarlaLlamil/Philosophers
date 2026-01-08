/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_table.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karlarod <karlarod@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 16:34:26 by karlarod          #+#    #+#             */
/*   Updated: 2026/01/08 16:49:40 by karlarod         ###   ########.fr       */
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
	fork = malloc(n_philo * sizeof(pthread_mutex_t));
	if (fork == NULL)
		return (NULL);
	while (i < n_philo)
	{
		pthread_mutex_init(&fork[i], NULL);
		++i;
	}
	return (fork);
}

int	init_status(t_status *status, int n_philo)
{
	struct timeval	start;
	int				i;

	i = 0;
	status->stop_simulation = false;
	gettimeofday(&start, NULL);
	status->start = start;
	while (i < n_philo)
	{
		status->forks_used[i] = -1;
		status->t_last_meal[i] = start;
		if (pthread_mutex_init(&status->mutx_last_meal[i], NULL) != 0)
			return (handle_mutex_init_fail(status->mutx_last_meal, i));
		++i;
	}
	if (pthread_mutex_init(&status->print, NULL) != 0)
		return (handle_mutex_init_fail(&status->print, 0));
	if (pthread_mutex_init(&status->stop, NULL) != 0)
		return (handle_mutex_init_fail(&status->stop, 0));
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
	{
		free(status->t_last_meal);
		return (free(status->forks_used), free(status), NULL);
	}
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
		table[i].index = i;
		table[i].l_fork = &forks[(i + 1) % input->n_philo];
		table[i].r_fork = &forks[i];
		table[i].status = status;
		++i;
	}
	return (table);
}
