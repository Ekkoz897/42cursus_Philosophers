/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apereira <apereira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 13:31:13 by apereira          #+#    #+#             */
/*   Updated: 2023/11/11 13:48:52 by apereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	check_condition(t_filo *filo)
{
	pthread_mutex_lock(&filo->config->life);
	if (filo->config->dead == 1 || filo->config->full == 1)
	{
		pthread_mutex_unlock(&filo->config->life);
		return (1);
	}
	pthread_mutex_unlock(&filo->config->life);
	return (0);
}


int	eating(t_filo *filo)
{
	int	*forks;

	forks = get_index_f(filo);
	if (!forks)
		return (1);
	if (locking_forks(filo, forks) == 1)
	{
		free(forks);
		return (1);
	}
	if (filo->config->dead == 1)
	{
		unlock_forks(filo, forks);
		return (1);
	}
	meal_time(filo);
	unlock_forks(filo, forks);
	free(forks);
	increase_meal(filo);
	if (check_condition(filo) == 1)
		return (1);
	return (0);
}

int	sleeping(t_filo *filo)
{
	print_status(filo, SLEEPING);
	sleep_time(filo->config->t_to_sleep);
	if (check_condition(filo) == 1)
		return (1);
	return (0);
}

// Se o tempo de pensar + tempo desde ultima comida
// for maior que t_to_die, ele so pensa 1 ms
int	thinking(t_filo *filo)
{
	unsigned long int	time;
	unsigned long int	time_to_think;

	time = get_current_time() - filo->last_meal;
	time_to_think = time / 2;
	if (time_to_think + time > (unsigned long int)filo->config->t_to_die)
		time_to_think = 1;
	print_status(filo, THINKING);
	usleep(time_to_think * 1000);
	if (check_condition(filo) == 1)
		return (1);
	return (0);
}
