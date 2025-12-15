#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdbool.h>
#include <sys/time.h>

typedef enum e_philo_status
{
	FORK,
	EAT,
	SLEEP,
	THINK,
	DEAD
} e_philo_status;

typedef	struct s_parameters
{
	double			time_die;
	double			time_eat;
	double			time_sleep;
	int				n_dinners;
	struct timeval	start;
}	t_parameters;

typedef struct s_status
{
	bool			stop_simulation;
	pthread_mutex_t	*print;
	pthread_mutex_t	*stop;

} t_status;

typedef struct s_philosophers
{
	int				index;
	t_status		*status;
	t_parameters	*conditions;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t *r_fork;
} t_philosophers;

void	*philos_routine(void *philos);
bool	validate_input(int n, char **argv);
bool	read_stop_simulation(t_status *status);
void	write_stop_simulation(t_philosophers *philo);
void	print_status(t_philosophers *philo, e_philo_status state);
int		ft_atoi(char *number);

#endif