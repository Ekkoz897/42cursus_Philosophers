/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apereira <apereira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 12:21:56 by apereira          #+#    #+#             */
/*   Updated: 2023/11/11 13:53:48 by apereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	exit_sim(pthread_t *threads, pthread_t police, t_config *config)
{
	if (pthread_join(police, NULL) != 0)
		return (0);
	destroy_sim(config, threads, config->forks);
	return (1);
}

void	destroy_sim(t_config *config, pthread_t *threads, pthread_mutex_t *frk)
{
	int	i;

	i = 0;
	while (i < config->num_filos)
	{
		if (pthread_join(threads[i], NULL) != 0)
			return ;
		i++;
	}
	destroy_mutexes(config, frk);
	free(threads);
	free_config(config, config->num_filos);
}

void	destroy_mutexes(t_config *config, pthread_mutex_t *forks)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&config->life);
	pthread_mutex_destroy(&config->printing);
	while (i < config->num_filos)
	{
		pthread_mutex_destroy(&forks[i]);
		pthread_mutex_destroy(&config->filos[i]->meal);
		pthread_mutex_destroy(&config->filos[i]->number);
		pthread_mutex_destroy(&config->searching[i]);
		i++;
	}
}

void	free_config(t_config *config, int num_philo)
{
	int	i;

	i = 0;
	while (i < num_philo)
	{
		if (config->filos[i])
			free(config->filos[i]);
		i++;
	}
	if (config->filos)
		free(config->filos);
	if (config->forks)
		free(config->forks);
	if (config->searching)
		free(config->searching);
	if (config->frk)
		free(config->frk);
	if (config)
		free(config);
}

void	sleep_time(int time_to_sleep)
{
	struct timeval	newtime;
	long			time;
	long			stop;

	if (gettimeofday(&newtime, NULL) != 0)
	{
		perror("gettimeofday");
		return ;
	}
	time = (newtime.tv_sec * 1000) + (newtime.tv_usec / 1000);
	stop = time + time_to_sleep;
	while (get_current_time() < stop)
		usleep(100);
}
