/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karlarod <karlarod@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 16:38:07 by karlarod          #+#    #+#             */
/*   Updated: 2026/01/08 16:56:21 by karlarod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdio.h>
#include <limits.h>
#include "philosophers.h"

bool	is_all_digit(char *str)
{
	int	j;

	j = 0;
	while (str[j] != '\0')
	{
		if (str[j] < '0' || str[j] > '9')
			return (false);
		++j;
	}
	return (true);
}

long int	ft_atoi(char *number)
{
	int			i;
	long int	num;

	i = 0;
	num = 0;
	while (number[i] != '\0')
	{
		num = num * 10 + number[i] - '0';
		++i;
	}
	if (num > INT_MAX)
		return (-1);
	return (num);
}

bool	validate_input(int n, char **argv, t_parameters *input)
{
	if (!is_all_digit(argv[1]) || !is_all_digit(argv[2])
		|| !is_all_digit(argv[3]) || !is_all_digit(argv[4]))
		return (false);
	input->n_philo = ft_atoi(argv[1]);
	if (input->n_philo < 0 || input->n_philo > 200)
		return (false);
	input->time_die = ft_atoi(argv[2]) * 1000;
	input->time_eat = ft_atoi(argv[3]) * 1000;
	input->time_sleep = ft_atoi(argv[4]) * 1000;
	if (input->time_die < 0 || input->time_eat < 0 || input->time_sleep < 0)
		return (false);
	if (n == 6)
	{
		if (!is_all_digit(argv[5]))
			return (false);
		input->n_dinners = ft_atoi(argv[5]);
		if (input->n_dinners < 0)
			return (false);
	}
	else
		input->n_dinners = -1;
	return (true);
}
