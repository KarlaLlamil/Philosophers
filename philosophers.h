#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdbool.h>
#include <sys/time.h>

typedef enum e_philo_status
{
	FORK_L,
	FORK_R,
	EAT,
	SLEEP,
	THINK,
	DEAD
} e_philo_status;

typedef	struct s_parameters
{
	int				n_philos;
	double			time_die;
	double			time_eat;
	double			time_sleep;
	int				n_dinners;
}	t_parameters;

typedef struct s_status
{
	bool			stop_simulation;
	int				*forks_used;
	int				starving_p;
	struct timeval	start;
	struct timeval	*t_last_meal;
	pthread_mutex_t	*mutx_last_meal;
	pthread_mutex_t	print;
	pthread_mutex_t	stop;

} t_status;

typedef struct s_philosophers
{
	int				index;
	t_status		*status;
	t_parameters	*conditions;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t *r_fork;
} t_philosophers;

typedef struct s_monitor
{
	t_parameters	*conditions;
	t_status		*status;
} t_monitor;

void	*philos_routine(void *philosopher);
void *monitorig_routine(void *status);
bool	validate_input(int n, char **argv);
bool	read_stop_simulation(t_status *status);
void	write_stop_simulation(int n, t_status *status);
void	print_status(int n, t_status *status, e_philo_status state);
int		ft_atoi(char *number);

#endif