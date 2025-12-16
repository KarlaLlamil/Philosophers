#include "philosophers.h"
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>

void	think(struct timeval *last_meal, t_philosophers *philo)
{
	struct timeval current;
	double			time_elapsed;

	//printf("think philo %i\n", ((t_philosophers *)philo)->index);
	if (read_stop_simulation(philo->status))
		return ;
	gettimeofday(&current, NULL);
	time_elapsed = (current.tv_sec - last_meal->tv_sec) * 1e6;
	time_elapsed = (time_elapsed + (current.tv_usec - last_meal->tv_usec));
	if (time_elapsed > philo->conditions->time_die)
	{
		write_stop_simulation(philo);
		return ;
	}
	print_status(philo, THINK);
}

bool	get_forks(t_philosophers *philo)
{
	//printf("get forks philo %i\n", ((t_philosophers *)philo)->index);
	if (read_stop_simulation(philo->status))
		return (false) ;
	if (philo->index%2 == 0)
	{
		pthread_mutex_lock(philo->r_fork);
		print_status(philo,FORK);
		//philo->status->forks_used[philo->index] = philo->index;
		pthread_mutex_lock(philo->l_fork);
		print_status(philo, FORK);
		//philo->status->forks_used[(philo->index + 1) % n_philo] = philo->index;
	}
	else
	{
		pthread_mutex_lock(philo->l_fork);
		print_status(philo, FORK);
		pthread_mutex_lock(philo->r_fork);
		print_status(philo, FORK);
		//philo->status->forks_used[philo->index] = philo->index;
	}
	return (true);
	// pthread_mutex_lock(&philo->print);
	// printf("forks picked up right-> %i left->%i\n",philo->index, left);
	// pthread_mutex_unlock(&philo->print);
}

void	put_forks(t_philosophers *philo)
{
	printf("put forks philo %i\n", ((t_philosophers *)philo)->index);
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

	//printf("eat philo %i\n", ((t_philosophers *)philo)->index);
	if (read_stop_simulation(philo->status))
		return ;
	gettimeofday(&current, NULL);
	time_elapsed = (current.tv_sec - last_meal->tv_sec) * 1e6;
	time_elapsed = (time_elapsed + (current.tv_usec - last_meal->tv_usec)) ;
	if (time_elapsed > philo->conditions->time_die)
	{
		write_stop_simulation(philo);
		return ;
	}
	print_status(philo, EAT);
	usleep(philo->conditions->time_eat);
	gettimeofday(last_meal, NULL);
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
	while (time_sleeping < philo->conditions->time_sleep && !read_stop_simulation(philo->status))
	{
		//printf("time that has been sleeping %f\n", time_sleeping);
		gettimeofday(&current, NULL);
		time_elapsed = (current.tv_sec - last_meal->tv_sec) * 1e6;
		time_elapsed = (time_elapsed + (current.tv_usec - last_meal->tv_usec));
		//printf ("time elapsed %f \n", time_elapsed);
		if (time_elapsed > philo->conditions->time_die)
		{
			//printf("philo %i die while sleeping\n", philo->index);
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
	// bool			have_forks;
	
	last_meal = ((t_philosophers *)philo)->conditions->start;
	//printf("start thread philo %i\n", ((t_philosophers *)philo)->index);
	while(!read_stop_simulation(((t_philosophers *)philo)->status))
	{
		if (((t_philosophers *)philo)->conditions->n_dinners >= 0 && n_meals == ((t_philosophers *)philo)->conditions->n_dinners)
			return (NULL);
		if (((t_philosophers*)philo)->l_fork != ((t_philosophers*)philo)->r_fork)
		{
			if (get_forks((t_philosophers *)philo))
			{
				eat (&last_meal, &n_meals, (t_philosophers*)philo);
				put_forks ((t_philosophers*)philo);
			}
		}
		f_sleep (&last_meal, (t_philosophers *)philo);
		think (&last_meal, (t_philosophers *)philo);
	}
	return (NULL);
	
}