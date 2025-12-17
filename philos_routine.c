#include "philosophers.h"
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>

void	think(t_philosophers *philo)
{
	// struct timeval current;
	// double			time_elapsed;

	//printf("think philo %i\n", ((t_philosophers *)philo)->index);
	if (read_stop_simulation(philo->status))
		return ;
	// gettimeofday(&current, NULL);
	// time_elapsed = (current.tv_sec - last_meal->tv_sec) * 1e6;
	// time_elapsed = (time_elapsed + (current.tv_usec - last_meal->tv_usec));
	// if (time_elapsed > philo->conditions->time_die)
	// {
	// 	write_stop_simulation(philo->index, philo->status);
	// 	return ;
	// }
	print_status(philo->index, philo->status, THINK);
}

int	get_forks(t_philosophers *philo)
{
	//printf("get forks philo %i\n", ((t_philosophers *)philo)->index);
	if (read_stop_simulation(philo->status))
		return (0) ;
	if (philo->index%2 == 0)
	{
		pthread_mutex_lock(philo->r_fork);
		philo->status->forks_used[philo->index] = philo->index;
		if (read_stop_simulation(philo->status) || philo->l_fork == philo->r_fork)
			return (1);
		print_status(philo->index, philo->status, FORK);
		//philo->status->forks_used[philo->index] = philo->index;
		pthread_mutex_lock(philo->l_fork);
		philo->status->forks_used[(philo->index + 1) % philo->conditions->n_philos] = philo->index;
		if (read_stop_simulation(philo->status))
			return (2);
		print_status(philo->index, philo->status, FORK);
		//philo->status->forks_used[(philo->index + 1) % n_philo] = philo->index;
	}
	else
	{
		pthread_mutex_lock(philo->l_fork);
		philo->status->forks_used[(philo->index + 1) % philo->conditions->n_philos] = philo->index;
		if (read_stop_simulation(philo->status) || philo->l_fork == philo->r_fork)
			return (1);
		print_status(philo->index, philo->status, FORK);
		pthread_mutex_lock(philo->r_fork);
		philo->status->forks_used[philo->index] = philo->index;
		if (read_stop_simulation(philo->status))
			return (2);
		print_status(philo->index, philo->status, FORK);
		//philo->status->forks_used[philo->index] = philo->index;
	}
	return (2);
	// pthread_mutex_lock(&philo->print);
	// printf("forks picked up right-> %i left->%i\n",philo->index, left);
	// pthread_mutex_unlock(&philo->print);
}

void	put_forks(int n_mut, t_philosophers *philo)
{
	//printf("put forks philo %i\n", ((t_philosophers *)philo)->index);
	if (philo->index%2 == 0 && n_mut != 0)
	{
		pthread_mutex_unlock(philo->r_fork);
		if (n_mut == 2)
			pthread_mutex_unlock(philo->l_fork);
	}
	else if (n_mut != 2)
	{
		pthread_mutex_unlock(philo->l_fork);
		if (n_mut != 2)
			pthread_mutex_unlock(philo->r_fork);
	}
}

void	eat( int  *n_meals, t_philosophers *philo)
{
	struct timeval current;
	double			time_elapsed;
	double			time_eating;
	double			timer;
	// bool			first;

	time_eating = 0;
	timer = 8000;
	if (philo->conditions->time_eat <  8000)
		timer = philo->conditions->time_eat;
	// first = true;
	if (read_stop_simulation(philo->status))
		return ;
	gettimeofday(&current, NULL);
	pthread_mutex_lock(&philo->status->mutx_last_meal[philo->index]);
	time_elapsed = (current.tv_sec - philo->status->t_last_meal[philo->index].tv_sec) * 1e6;
	time_elapsed = (time_elapsed + (current.tv_usec - philo->status->t_last_meal[philo->index].tv_usec)) ;
	pthread_mutex_unlock(&philo->status->mutx_last_meal[philo->index]);
	if (time_elapsed >= philo->conditions->time_die)
	{
		write_stop_simulation(philo->index, philo->status);
		return ;
	}
	//print_status(philo->index, philo->status, EAT);
	//usleep(philo->conditions->time_eat);
	pthread_mutex_lock(&philo->status->mutx_last_meal[philo->index]);
	philo->status->t_last_meal[philo->index] = current;
	pthread_mutex_unlock(&philo->status->mutx_last_meal[philo->index]);
	print_status(philo->index, philo->status, EAT);
	while (time_eating < philo->conditions->time_eat && !read_stop_simulation(philo->status))
	{
		//printf("time that has been sleeping %f\n", time_sleeping);
		// gettimeofday(&current, NULL);
		// time_elapsed = (current.tv_sec - last_meal->tv_sec) * 1e6;
		// time_elapsed = (time_elapsed + (current.tv_usec - last_meal->tv_usec));
		// //printf ("time elapsed %f \n", time_elapsed);
		// if (time_elapsed > philo->conditions->time_die)
		// {
		// 	//printf("philo %i die while sleeping\n", philo->index);
		// 	write_stop_simulation(philo->index, philo->status);
		// 	return ;
		// }
		// if (first == true)
		// 	print_status(philo->index, philo->status, EAT);
		// first = false;
		usleep(timer);
		time_eating = time_eating + timer;
		if (time_eating + timer > philo->conditions->time_sleep)
			timer = philo->conditions->time_sleep - time_eating;
	}
	gettimeofday(&current, NULL);
	pthread_mutex_lock(&philo->status->mutx_last_meal[philo->index]);
	philo->status->t_last_meal[philo->index] = current;
	pthread_mutex_unlock(&philo->status->mutx_last_meal[philo->index]);

	//printf("eat philo %i\n", ((t_philosophers *)philo)->index);
	// if (read_stop_simulation(philo->status))
	// 	return ;
	// gettimeofday(&current, NULL);
	// time_elapsed = (current.tv_sec - last_meal->tv_sec) * 1e6;
	// time_elapsed = (time_elapsed + (current.tv_usec - last_meal->tv_usec)) ;
	// if (time_elapsed > philo->conditions->time_die)
	// {
	// 	write_stop_simulation(philo->index, philo->status);
	// 	return ;
	// }
	// print_status(philo->index, philo->status, EAT);
	// usleep(philo->conditions->time_eat);
	
	// gettimeofday(last_meal, NULL);
	if (time_eating >= philo->conditions->time_eat)
		++(*n_meals);
}

void	f_sleep( t_philosophers *philo)
{
	//struct timeval current;
	//double			time_elapsed;
	double			time_sleeping;
	double			timer;
	bool			first;

	time_sleeping = 0;
	timer = 8000;
	if (philo->conditions->time_sleep <  8000)
		timer = philo->conditions->time_sleep;
	first = true;
	// if (read_stop_simulation(philo->status))
	// 	return ;
	while (time_sleeping < philo->conditions->time_sleep && !read_stop_simulation(philo->status))
	{
		//printf("time that has been sleeping %f\n", time_sleeping);
		// gettimeofday(&current, NULL);
		// time_elapsed = (current.tv_sec - last_meal->tv_sec) * 1e6;
		// time_elapsed = (time_elapsed + (current.tv_usec - last_meal->tv_usec));
		// //printf ("time elapsed %f \n", time_elapsed);
		// if (time_elapsed > philo->conditions->time_die)
		// {
		// 	//printf("philo %i die while sleeping\n", philo->index);
		// 	write_stop_simulation(philo->index, philo->status);
		// 	return ;
		// }
		if (first == true)
			print_status(philo->index, philo->status, SLEEP);
		first = false;
		usleep(timer);
		time_sleeping = time_sleeping + timer;
		if (time_sleeping + timer > philo->conditions->time_sleep)
			timer = philo->conditions->time_sleep - time_sleeping;
	}
}

void	*philos_routine(void *philo)
{
	int				n_meals;
	int				n_mtx;
	// bool			have_forks;

	//printf("start thread philo %i\n", ((t_philosophers *)philo)->index);
	while(!read_stop_simulation(((t_philosophers *)philo)->status))
	{
		if (((t_philosophers *)philo)->conditions->n_dinners >= 0 && n_meals == ((t_philosophers *)philo)->conditions->n_dinners)
			return (NULL);
		// Revisar si considerar que tome un tenedor
		n_mtx = get_forks((t_philosophers *)philo);
		if (n_mtx == 2)
			eat (&n_meals, (t_philosophers*)philo);
		put_forks (n_mtx, (t_philosophers*)philo);
		f_sleep ( (t_philosophers *)philo);
		think ( (t_philosophers *)philo);
	}
	return (NULL);
	
}