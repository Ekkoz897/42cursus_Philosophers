/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   police.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apereira <apereira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 12:38:44 by apereira          #+#    #+#             */
/*   Updated: 2023/11/11 13:56:13 by apereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	*policia(void *config)
{
	int			i;
	t_config	*config_cpy;

	config_cpy = (t_config *)config;
	while (1)
	{
		i = 0;
		if (check_all_max_meals(config_cpy) == 0)
			return (NULL);
		while (i < config_cpy->num_filos)
		{
			if (check_death(config_cpy->filos[i]) == 1)
				return (NULL);
			i++;
		}
	}
}

int	check_all_max_meals(t_config *config)
{
	int	i;
	int	full;

	i = 0;
	full = 0;
	while (i < config->num_filos)
	{
		if (check_max_meals(config->filos[i]) == 1)
			full += 1;
		else
			return (1);
		i++;
	}
	if (full == config->num_filos)
	{
		pthread_mutex_lock(&config->life);
		config->full = 1;
		pthread_mutex_unlock(&config->life);
		return (0);
	}
	return (1);
}

int	check_max_meals(t_filo *filo)
{
	pthread_mutex_lock(&filo->number);
	if (filo->meal_nbr == filo->config->max_meals)
	{
		pthread_mutex_unlock(&filo->number);
		return (1);
	}
	pthread_mutex_unlock(&filo->number);
	return (0);
}

int	check_death(t_filo *filo)
{
	pthread_mutex_lock(&filo->meal);
	if (get_current_time() - filo->last_meal >= filo->config->t_to_die)
	{
		pthread_mutex_lock(&filo->config->life);
		filo->config->dead = 1;
		pthread_mutex_unlock(&filo->config->life);
		print_status(filo, DEAD);
		pthread_mutex_unlock(&filo->meal);
		return (1);
	}
	pthread_mutex_unlock(&filo->meal);
	return (0);
}

// Used for the print states function
int	check_if_dead(t_filo *filo)
{
	pthread_mutex_lock(&filo->config->life);
	if (filo->config->dead == 1)
	{
		pthread_mutex_unlock(&filo->config->life);
		return (1);
	}
	pthread_mutex_unlock(&filo->config->life);
	return (0);
}
