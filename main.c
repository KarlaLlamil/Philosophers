/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karlarod <karlarod@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 16:04:56 by karlarod          #+#    #+#             */
/*   Updated: 2025/12/11 17:37:22 by karlarod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "philosophers.h"

void	philos_routine(void *table_data)
{
	t_philosophers *table;
	
	table = (t_philosophers *)table_data;
	if (table->index % 2 == 0)
		pthread_mutex_lock(&table->forks[table->index]);
	else if (table->index == table->n_philosophers)
		pthread_mutex_lock(&table->forks[])
	
}

int	init_philo(int n, t_input input)
{
	int				i;
	pthread_t		*philos;
	t_philosophers	*table;

	i = 0;
	philos = malloc(input[0] * sizeof(pthread_t));
	table = malloc(sizeof(t_philosophers));
	table->forks = malloc(input[0] * sizeof(pthread_mutex_t));
	table->input = input;
	while (i < n)
	{
		table->index = i;
		pthread_create(&philos[i], NULL, philos_routine, (void *)table);
		++i;
	}
}

int is_digit(int c)
{
	if (c >= 0 && c <= 9)
		return (0);
	return (1);
}
int		ft_atoi(char *number)
{
	int	i;
	int	num;

	i = 0;
	num = 0;
	while (number[i] != '\0')
	{
		num = num * 10 + number[i] - '0';
		++i;
	}
	
}

bool	validate_input(int n, char **argv, t_input *input)
{
	int	i;
	int	j;

	i = 1;
	while (i <= n)
	{
		j = 0;
		while (argv[i][j] != '\0')
		{
			if (is_digit(argv[i][j]) != 0)
				return (false);
			++j;
		}
		++i;
	}
	return (true);
}

int	main(int argc, char **argv)
{
	t_input	initial_conditions;

	initial_conditions = (t_input){};
	if (argc < 5 || argc > 6)
		printf("The program needs number of philosophers time ...");
	else if (validate_input(argc, argv) == false)
		printf("The input needs to be an int ");
	else
	{
		initial_conditions.n_philosophers = ft_atoi(argv[1]);
		initial_conditions.time_die = ft_atoi(argv[2]);
		initial_conditions.time_eat = ft_atoi(argv[3]);
		initial_conditions.time_sleep = ft_atoi(argv[4]);
		if (argc == 6)
			initial_conditions.n_dinners = ft_atoi(argv[5]);
		init_philo(argc - 1, initial_conditions);
	}
}
