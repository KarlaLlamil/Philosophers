/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karlarod <karlarod@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 16:56:56 by karlarod          #+#    #+#             */
/*   Updated: 2026/01/08 17:01:54 by karlarod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdbool.h>
# include <sys/time.h>
# define PHILO_MAX 200

typedef enum e_philo_action
{
	FORK,
	EAT,
	SLEEP,
	THINK,
	DEAD
}	t_philo_action;

typedef struct s_parameters
{
	int				n_philo;
	long int		time_die;
	long int		time_eat;
	long int		time_sleep;
	int				n_dinners;
}	t_parameters;

typedef struct s_status
{
	bool			stop_simulation;
	int				*forks_used;
	struct timeval	start;
	struct timeval	*t_last_meal;
	pthread_mutex_t	*mutx_last_meal;
	pthread_mutex_t	print;
	pthread_mutex_t	stop;
	int				n_meals[PHILO_MAX];
}	t_status;

typedef struct s_philosophers
{
	int				index;
	t_status		*status;
	t_parameters	*param;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
}	t_philosophers;

typedef struct s_monitor
{
	t_parameters	*param;
	t_status		*status;
}	t_monitor;

void			*philos_routine(void *philosopher);
void			*monitorig_routine(void *status);
bool			validate_input(int n, char **argv, t_parameters *input);
bool			read_stop_simulation(t_status *status);
void			write_stop_simulation(int n, t_status *status);
void			print_status(int n, t_status *status, t_philo_action state);
pthread_mutex_t	*make_mutex_forks(int n_philo);
t_philosophers	*make_table(t_parameters *input, pthread_mutex_t *forks);
void			*destroy_status(t_status *status, int n_philo, int type);
void			destroy_mutex_forks(int n_philo, pthread_mutex_t *fork);
int				handle_mutex_init_fail(pthread_mutex_t *mtx, int n);

#endif