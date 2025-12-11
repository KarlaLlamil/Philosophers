#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>

typedef	struct s_input
{
	int				n_philosophers;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				n_dinners;
}	t_input;


typedef struct s_philosophers
{
	int				index;
	t_input			input;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print;

} t_philosophers;

#endif