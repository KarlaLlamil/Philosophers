#include "philosophers.h"
#include <sys/time.h>
#include <unistd.h>

void	think(struct timeval *last_meal, t_philosophers *philo)
{
	struct timeval current;
	double			time_elapsed;

	if (read_stop_simulation(philo->status))
		return ;
	gettimeofday(&current, NULL);
	time_elapsed = (current.tv_sec - last_meal->tv_sec) * 1e6;
	time_elapsed = (time_elapsed + (current.tv_usec - last_meal->tv_usec)) * 1e-6;
	if (time_elapsed > philo->conditions->time_die)
	{
		write_stop_simulation(philo);
		return ;
	}
	print_status(philo, THINK);
}

void	get_forks(t_philosophers *philo)
{
	if (read_stop_simulation(philo->status))
		return ;
	if (philo->index%2 == 0)
	{
		pthread_mutex_lock(philo->r_fork);
		pthread_mutex_lock(philo->l_fork);
	}
	else
	{
		pthread_mutex_lock(philo->l_fork);
		pthread_mutex_lock(philo->r_fork);
	}
	// pthread_mutex_lock(&philo->print);
	// printf("forks picked up right-> %i left->%i\n",philo->index, left);
	// pthread_mutex_unlock(&philo->print);
}

void	put_forks(t_philosophers *philo)
{
	// if (philo->index%2 == 0)
	// {
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
	// }
	// else
	// {
	// 	pthread_mutex_unlock(&philo->forks[left]);
	// 	pthread_mutex_unlock(&philo->forks[philo->index]);
	// }
	//pthread_mutex_lock(&philo->print);
	//printf("forks put down\n");
	//pthread_mutex_unlock(&philo->print);

}

void	eat(struct timeval *last_meal, int  *n_meals, t_philosophers *philo)
{
	struct timeval current;
	double			time_elapsed;

	if (read_stop_simulation(philo->status))
		return ;
	gettimeofday(&current, NULL);
	time_elapsed = (current.tv_sec - last_meal->tv_sec) * 1e6;
	time_elapsed = (time_elapsed + (current.tv_usec - last_meal->tv_usec)) * 1e-6;
	if (time_elapsed > philo->conditions->time_die)
	{
		write_stop_simulation(philo);
		return ;
	}
	print_status(philo, EAT);
	usleep(philo->conditions->time_eat);
	++(*n_meals);
}

void	f_sleep(struct timeval *last_meal, t_philosophers *philo)
{
	struct timeval current;
	double			time_elapsed;
	double			time_sleeping;
	double			timer;
	bool			first;

	time_sleeping = 0;
	timer = 8000;
	if (philo->conditions->time_sleep <  8000)
		timer = philo->conditions->time_sleep;
	first = true;
	if (read_stop_simulation(philo->status))
		return ;
	while (time_sleeping < philo->conditions->time_sleep)
	{
		gettimeofday(&current, NULL);
		time_elapsed = (current.tv_sec - last_meal->tv_sec) * 1e6;
		time_elapsed = (time_elapsed + (current.tv_usec - last_meal->tv_usec)) * 1e-6;
		if (time_elapsed > philo->conditions->time_die)
		{
			write_stop_simulation(philo);
			return ;
		}
		if (first == true)
			print_status(philo, SLEEP);
		first = false;
		usleep(timer);
		time_sleeping = time_sleeping + timer;
		if (time_sleeping + timer > philo->conditions->time_sleep)
			timer = philo->conditions->time_sleep - time_sleeping;
	}
}

void	*philos_routine(void *philo)
{
	struct timeval	last_meal;
	int				n_meals;
	
	last_meal = ((t_philosophers *)philo)->conditions->start;
	while(read_stop_simulation(((t_philosophers *)philo)->status))
	{
		get_forks((t_philosophers *)philo);
		eat (&last_meal, &n_meals, (t_philosophers*)philo);
		put_forks ((t_philosophers*)philo);
		f_sleep (&last_meal, (t_philosophers *)philo);
		think (&last_meal, (t_philosophers *)philo);
	}
	return (NULL);
	
}