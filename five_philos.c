#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>



typedef struct s_philosophers
{
	int				index;
	pthread_mutex_t	*forks;
	pthread_mutex_t	*print;
	// deben ser punteros, es incorrecto eniar copias del mutex
	//pthread_mutex_t read;

} t_philosophers;

void	init_mutex_forks(pthread_mutex_t *fork, pthread_mutex_t *print)
{
	int	i;

	i = 0;
	while (i < 5)
	{
		pthread_mutex_init(&fork[i], NULL);
		++i;
	}
	pthread_mutex_init(print, NULL);
	//pthread_mutex_init(&table->read, NULL);
}

void	destroy_mutex_forks(pthread_mutex_t *fork, pthread_mutex_t *print)
{
	int	i;

	i = 0;
	while (i < 5)
	{
		pthread_mutex_destroy(&fork[i]);
		++i;
	}
	pthread_mutex_destroy(print);
	//pthread_mutex_destroy(&table->read);
}
void	think(t_philosophers *table)
{
	pthread_mutex_lock(table->print);
	printf("philosopher %i is thinking\n", table->index);
	//sleep(1);
	pthread_mutex_unlock(table->print);

	return ;
}

void	get_forks(t_philosophers *table)
{
	int	left;

	left = (table->index + 1)%5;
	if (table->index%2 == 0)
	{
		pthread_mutex_lock(&table->forks[table->index]);
		pthread_mutex_lock(&table->forks[left]);
	}
	else
	{
		pthread_mutex_lock(&table->forks[left]);
		pthread_mutex_lock(&table->forks[table->index]);
	}
	// pthread_mutex_lock(&table->print);
	// printf("forks picked up right-> %i left->%i\n",table->index, left);
	// pthread_mutex_unlock(&table->print);
}

void	put_forks(t_philosophers *table)
{
	int	left;

	left = (table->index + 1)%5;
	if (table->index%2 == 0)
	{
		pthread_mutex_unlock(&table->forks[table->index]);
		pthread_mutex_unlock(&table->forks[left]);
	}
	else
	{
		pthread_mutex_unlock(&table->forks[left]);
		pthread_mutex_unlock(&table->forks[table->index]);
	}
	//pthread_mutex_lock(&table->print);
	//printf("forks put down\n");
	//pthread_mutex_unlock(&table->print);

}

void	eat(t_philosophers *table)
{
	pthread_mutex_lock(table->print);
	printf("philosopher %i is eating\n", table->index);
	//sleep(1);
	pthread_mutex_unlock(table->print);
}

void	*philos_routine(void *table)
{
	while(1)
	{
		think ((t_philosophers *)table);
		//pthread_mutex_lock(&(((t_philosophers *)table)->read));
		get_forks((t_philosophers *)table);
		eat ((t_philosophers*)table);
		put_forks ((t_philosophers*)table);
		//pthread_mutex_lock(&(((t_philosophers *)table)->read));
	}
	return (NULL);
}

int	main(void)
{
	int	i;
	pthread_t		*philos;
	t_philosophers	**table;
	pthread_mutex_t *fork;
	pthread_mutex_t print;

	i = 0;
	philos = malloc(5 * sizeof(pthread_t));
	table = malloc(5 * sizeof(t_philosophers));
	fork = malloc(5 * sizeof(pthread_mutex_t));
	init_mutex_forks(fork, &print);
	while (i < 5)
	{
		table[i] = malloc(sizeof(t_philosophers));
		table[i]->forks = fork;
		table[i]->print = &print;
		//pthread_mutex_lock(&table->read);
		table[i]->index = i;
		//pthread_mutex_unlock(&table->read);
		pthread_create(&philos[i], NULL, philos_routine, (void *)table[i]);
		++i;
	}
	while (i > 0)
	{
		--i;
		//table->index = i;
		pthread_join(philos[i], NULL);
	}
	destroy_mutex_forks(fork, &print);
	return (0);
}