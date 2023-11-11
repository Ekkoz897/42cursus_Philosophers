/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apereira <apereira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 13:39:29 by apereira          #+#    #+#             */
/*   Updated: 2023/11/11 13:46:30 by apereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	*get_index_f(t_filo *filo)
{
	int	*forks;

	forks = malloc(sizeof(int) * 2);
	if (!forks)
		return (NULL);
	if (filo->id % 2 != 0)
	{
		forks[0] = filo->id - 1;
		if (filo->id == filo->config->num_filos)
			forks[1] = 0;
		else
			forks[1] = filo->id;
	}
	else
	{
		forks[1] = filo->id - 1;
		if (filo->id == filo->config->num_filos)
			forks[0] = 0;
		else
			forks[0] = filo->id;
	}
	return (forks);
}

int	locking_forks(t_filo *filo, int *forks)
{
	pthread_mutex_lock(&filo->config->forks[forks[0]]);
	print_status(filo, FORK);
	filo->config->frk[forks[0]] = 1;
	pthread_mutex_lock(&filo->config->forks[forks[1]]);
	print_status(filo, FORK);
	filo->config->frk[forks[1]] = 1;
	return (0);
}

void	unlock_forks(t_filo *filo, int *forks)
{
	pthread_mutex_unlock(&filo->config->forks[forks[0]]);
	pthread_mutex_unlock(&filo->config->forks[forks[1]]);
	pthread_mutex_lock(&filo->config->searching[forks[0]]);
	pthread_mutex_lock(&filo->config->searching[forks[1]]);
	filo->config->frk[forks[0]] = 0;
	filo->config->frk[forks[1]] = 0;
	pthread_mutex_unlock(&filo->config->searching[forks[0]]);
	pthread_mutex_unlock(&filo->config->searching[forks[1]]);
}

void	meal_time(t_filo *filo)
{
	pthread_mutex_lock(&filo->meal);
	print_status(filo, EATING);
	filo->last_meal = get_current_time();
	pthread_mutex_unlock(&filo->meal);
	sleep_time(filo->config->t_to_eat);
}

void	increase_meal(t_filo *filo)
{
	if (filo->config->max_meals != -1)
	{
		pthread_mutex_lock(&filo->number);
		filo->meal_nbr = filo->meal_nbr + 1;
		pthread_mutex_unlock(&filo->number);
	}
}
