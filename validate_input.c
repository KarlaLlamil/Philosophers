/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karlarod <karlarod@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 16:38:07 by karlarod          #+#    #+#             */
/*   Updated: 2025/12/15 16:38:08 by karlarod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdio.h>

int is_digit(int c)
{
	if (c >= '0' && c <= '9')
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
	return (num);
}

bool	validate_input(int n, char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (i < n)
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