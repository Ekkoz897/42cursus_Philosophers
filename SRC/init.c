/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apereira <apereira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 11:59:39 by apereira          #+#    #+#             */
/*   Updated: 2023/11/14 17:44:52 by apereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

t_config	*init(int ac, char **av)
{
	t_config	*config;

	if (validator(ac, av) == -2)
		return (NULL);
	config = malloc(sizeof(t_config));
	if (!config)
		return (NULL);
	config = init_values(config, ac, av);
	if (!config)
		return (NULL);
	config = init_filos(config, ft_atoi(av[1]));
	if (!config)
		return (NULL);
	config = allocate_mut(config, ft_atoi(av[1]));
	if (!config)
		return (NULL);
	init_mutexes(config);
	return (config);
}

t_config	*init_values(t_config *config, int ac, char **av)
{
	config->start_time = get_current_time();
	config->num_filos = ft_atoi(av[1]);
	config->t_to_die = ft_atoi(av[2]);
	config->t_to_eat = ft_atoi(av[3]);
	config->t_to_sleep = ft_atoi(av[4]);
	config->max_meals = validator(ac, av);
	config->dead = 0;
	config->full = 0;
	config->frk = malloc(sizeof(int) * ft_atoi(av[1]));
	if (!config->frk)
		return (NULL);
	return (config);
}

t_config	*init_filos(t_config *config, int num_filos)
{
	int	i;

	config->filos = malloc(sizeof(t_filo) * num_filos + 1);
	if (!config->filos)
		return (NULL);
	i = 0;
	while (i < num_filos)
	{
		config->filos[i] = malloc(sizeof(t_filo));
		if (!config->filos[i])
			return (NULL);
		config->filos[i]->id = i + 1;
		config->filos[i]->config = config;
		config->filos[i]->last_meal = config->start_time;
		config->filos[i]->meal_nbr = 0;
		config->frk[i] = 0;
		i++;
	}
	config->filos[i] = NULL;
	return (config);
}

void	init_mutexes(t_config *config)
{
	int	i;

	i = 0;
	while (i < config->num_filos)
	{
		pthread_mutex_init(&config->forks[i], NULL);
		pthread_mutex_init(&config->filos[i]->meal, NULL);
		pthread_mutex_init(&config->filos[i]->number, NULL);
		pthread_mutex_init(&config->searching[i], NULL);
		i++;
	}
	pthread_mutex_init(&config->life, NULL);
	pthread_mutex_init(&config->printing, NULL);
}

t_config	*allocate_mut(t_config *config, int num_filos)
{
	config->forks = malloc(sizeof(pthread_mutex_t) * num_filos);
	if (!config->forks || !config)
		return (NULL);
	config->searching = malloc(sizeof(pthread_mutex_t) * num_filos);
	if (!config->searching)
		return (NULL);
	return (config);
}
