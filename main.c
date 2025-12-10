/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karlarod <karlarod@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 16:04:56 by karlarod          #+#    #+#             */
/*   Updated: 2025/12/10 16:39:26 by karlarod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdbool.h>

int is_digit(int c)
{
	if (c >= 0 && c <= 9)
		return (0);
	return (1);
}

bool	validate_input(int n, char **argv, int input[5])
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
				return(false);
			input[i - 1] = input[i - 1]*10 + argv[i][j] - '0';
			++j;
		}
		++i;
	}
}

int	main(int argc, char **argv)
{
	int	input[5];

	memset(input, 0, sizeof(input));
	if (argc < 5 || argc > 6)
		printf("The program needs number of philosophers time ...");
	else if (validate_input(argc, argv, input) == false)
		printf("The input needs to be an int ");
	else
		init_philo(input);
}