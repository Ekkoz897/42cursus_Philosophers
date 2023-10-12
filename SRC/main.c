/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apereira <apereira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 10:02:26 by apereira          #+#    #+#             */
/*   Updated: 2023/10/12 12:39:20 by apereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

t_vars	*init_vars(char **av, t_vars *vars)
{
	vars = malloc(sizeof(t_vars));
	if (!vars)
		return (NULL);
	vars->filo = malloc(sizeof(t_filo));
	if (!vars->filo)
	{
		free(vars);
		return (NULL);
	}
	vars->filo->n_filos = ft_atoi(av[1]);
	vars->filo->t_to_die = ft_atoi(av[2]);
	vars->filo->t_to_eat = ft_atoi(av[3]);
	vars->filo->t_to_sleep = ft_atoi(av[4]);
	if (av[5])
		vars->min_meals = ft_atoi(av[5]);
	return (vars);
}

// Implementation of the philosopher's behavior
// usleep funciona em microsegundos, logo, 1000 = 1ms
// para evitar deadlock, fazemos com q o ultimo pegue no garfo da direita
void	*ft_filo(void *data)
{
	t_filo	*filo;

	filo = (t_filo *)data;
	while (1)
	{
		// pensar
		printf("Philosopher %d is thinking\n", (filo->id));
		// tentar comer
		if (filo->id == filo->n_filos - 1)
			ft_pick_right_fork(filo, data);
		else
		{
			pthread_mutex_lock(filo->left_fork);
			printf("Philosopher %d grabbed the left fork\n", filo->id);
			pthread_mutex_lock(filo->right_fork);
			printf("Philosopher %d grabbed the right fork\n", filo->id);
		}
		printf("Philosopher %d is eating\n", filo->id);
		filo->last_meal = get_current_time();
		usleep(filo->t_to_eat * 1000);
		pthread_mutex_unlock(filo->left_fork);
		pthread_mutex_unlock(filo->right_fork);
		// dormir
		printf("Philosopher %d is sleeping\n", filo->id);
		usleep(filo->t_to_sleep * 1000);
		// Verificar se esta morto (later)
	}
	return (NULL);
}

// philosophers[i].right_fork = &forks[(i + 1) % N] -> para ter
// certeza que o ultimo filosofo pega no primeiro garfo com a mao direita
void	ft_start_simulation(t_vars *vars, pthread_mutex_t *forks,
		pthread_t *threads)
{
	int	i;

	i = 0;
	while (i < vars->filo->n_filos)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	i = 0;
	while (i < vars->filo->n_filos)
	{
		vars->filo[i].id = i;
		vars->filo[i].state = THINK;
		vars->filo[i].meals_eaten = 0;
		vars->filo[i].last_meal = 0;
		vars->filo[i].left_fork = &forks[i];
		vars->filo[i].right_fork = &forks[(i + 1) % vars->filo->n_filos];
		pthread_create(&threads[i], NULL, &ft_filo, &vars->filo[i]);
		i++;
	}
}

// pthread_join espera que as threads terminem
int	main(int ac, char **av)
{
	t_vars			*vars;
	pthread_mutex_t	*forks;
	pthread_t		*threads;
	int				i;

	if (ac < 5 || ac > 6)
	{
		printf("Error: Wrong number of arguments\n");
		return (1);
	}
	vars = NULL;
	vars = init_vars(av, vars);
	forks = malloc(ft_atoi(av[1]) * sizeof(pthread_mutex_t));
	threads = malloc(ft_atoi(av[1]) * sizeof(pthread_t));
	if (!vars->filo || !forks || !threads)
		return (1);
	ft_start_simulation(vars, forks, threads);
	i = 0;
	while (i < vars->filo->n_filos)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	free (forks);
	free (threads);
	free(vars->filo);
	free(vars);
	return (0);
}
