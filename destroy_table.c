/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy_table.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karlarod <karlarod@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 16:35:36 by karlarod          #+#    #+#             */
/*   Updated: 2026/01/08 16:51:20 by karlarod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "philosophers.h"

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
}

void	*destroy_status(t_status *status, int n_philo, int type)
{
	int				i;

	i = 0;
	if (status == NULL)
		return (NULL);
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
		while (i < n)
		{
			pthread_mutex_destroy(&mtx[i]);
			++i;
		}
	}
	return (1);
}
