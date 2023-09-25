/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apereira <apereira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 10:02:26 by apereira          #+#    #+#             */
/*   Updated: 2023/09/25 14:11:29 by apereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

t_vars	*init_vars(char **av, t_vars *vars)
{
	vars = malloc(sizeof(t_vars));
	if (!vars)
		return (NULL);
	vars->n_filos = ft_atoi(av[1]);
	vars->t_to_die = ft_atoi(av[2]);
	vars->t_to_eat = ft_atoi(av[3]);
	vars->t_to_sleep = ft_atoi(av[4]);
	if (av[5])
		vars->min_meals = ft_atoi(av[5]);
	return (vars);
}

// ... Implementation of the philosopher's behavior ...
void	*ft_filo(void *philo)
{
	return (NULL);
}

// philosophers[i].right_fork = &forks[(i + 1) % N] -> para ter
// certeza que o ultimo filosofo pega no primeiro garfo com a mao direita
void	ft_start_simulation(t_vars *vars, pthread_mutex_t *forks,
		pthread_t *threads)
{
	int	i;

	i = -1;
	while (i < vars->n_filos)
		pthread_mutex_init(&forks[i++], NULL);
	i = -1;
	while (++i < vars->n_filos)
	{
		vars->filo[i].id = i;
		vars->filo[i].state = THINK;
		vars->filo[i].meals_eaten = 0;
		vars->filo[i].last_meal = 0;
		vars->filo[i].left_fork = &forks[i];
		vars->filo[i].right_fork = &forks[(i + 1) % vars->n_filos];
		pthread_mutex_init(&forks[i], NULL);
		pthread_create(&threads[i], NULL, &ft_filo, &vars->filo[i]);
	}
}

int	main(int ac, char **av)
{
	t_vars			*vars;
	pthread_mutex_t	*forks;
	pthread_t		*threads;

	if (ac < 5 || ac > 6)
	{
		printf("Error: Wrong number of arguments\n");
		return (1);
	}
	vars = NULL;
	vars = init_vars(av, vars);
	vars->filo = malloc(ft_atoi(av[1]) * sizeof(t_filo));
	forks = malloc(ft_atoi(av[1]) * sizeof(pthread_mutex_t));
	threads = malloc(ft_atoi(av[1]) * sizeof(pthread_t));
	if (!vars->filo || !forks || !threads)
		return (1);
	ft_start_simulation(vars, forks, threads);
	printf("%s\n", av[1]);
	free (forks);
	free (threads);
	return (0);
}
