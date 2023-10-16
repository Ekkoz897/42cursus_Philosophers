/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apereira <apereira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 10:21:20 by apereira          #+#    #+#             */
/*   Updated: 2023/10/16 15:37:13 by apereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

long	ft_atoi(const char *str)
{
	long long	res;
	int			i;
	int			posneg;

	res = 0;
	i = 0;
	posneg = 1;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			posneg = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - '0');
		i++;
	}
	return (res * posneg);
}

// struct timevall guarda o tempo em segundos e microsegundos
long	get_current_time(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}

void	ft_pick_right_fork(t_philo *filo,void *data)
{
	pthread_mutex_lock(((t_philo *)filo)->right_fork);
	printf("P%d grabbed the right fork\n", ((t_philo *)data)->id);
    pthread_mutex_lock(((t_philo *)filo)->left_fork);
    printf("P%d grabbed the left fork\n", ((t_philo *)data)->id);
}

t_config	*init_vars(int ac, char **av)
{
	if (!args_ok(ac, av))
		return (NULL);
	t_config *config = malloc(sizeof(t_config));
	if (!config)
		return (NULL);
	config->n_filos = ft_atoi(av[1]);
	config->t_to_die = ft_atoi(av[2]);
	config->t_to_eat = ft_atoi(av[3]);
	config->t_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		config->max_meals = ft_atoi(av[5]);
	else
		config->max_meals = -1;
	
	return (config);
}

void ft_clear_mem(t_philo *filo, pthread_mutex_t *forks, t_config *config,
		pthread_t *threads)
{
	int i;

	i = 0;
	while (i < config->n_filos)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	i = 0;
	while (i < config->n_filos)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
	free(forks);
	free(filo);
	free(config);
	free (threads);
}

int	args_ok(int ac, char **av)
{
	int i;
	int	j;

	if (ac < 5 || ac > 6)
	{
		printf("Wrong number of arguments\n");
		return (0);
	}
	i = 1;
	while (i < ac)
	{
		if (ft_atoi(av[i]) < 0)
			return (0);
		j = 0;
		while (av[i][j])
		{
			if (av[i][j] < '0' || av[i][j] > '9')
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

void mutex_destroyer(pthread_mutex_t *forks, t_philo *filo, t_config *config)
{
	int i;

	i = 0;
	while (i < config->n_filos)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
	pthread_mutex_destroy(&filo->config->permission_to_pick_forks);
}
