/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apereira <apereira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 10:21:20 by apereira          #+#    #+#             */
/*   Updated: 2023/10/27 15:25:17 by apereira         ###   ########.fr       */
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

void	ft_pick_right_fork(t_philo *filo)
{
	if (pthread_mutex_lock(filo->right_fork) != 0)
	{
		pthread_mutex_unlock(&filo->config->permission_to_pick_forks);
		usleep(1 * 1000);
		return ;
	}
	print_status(filo, RFORK);
	if (pthread_mutex_lock(filo->left_fork) != 0)
	{
		pthread_mutex_unlock(filo->right_fork);
		print_status(filo, DRFORK);
		pthread_mutex_unlock(&filo->config->permission_to_pick_forks);
		usleep(1 * 1000);
		return ;
	}
	print_status(filo, LFORK);
}

t_config	*init_vars(int ac, char **av)
{
	t_config	*config;

	if (!args_ok(ac, av))
		return (NULL);
	config = malloc(sizeof(t_config));
	if (!config)
		return (NULL);
	config->n_filos = ft_atoi(av[1]);
	config->t_to_die = ft_atoi(av[2]);
	config->t_to_eat = ft_atoi(av[3]);
	config->t_to_sleep = ft_atoi(av[4]);
	config->start_time = get_current_time();
	if (ac == 6)
		config->max_meals = ft_atoi(av[5]);
	else
		config->max_meals = -1;
	return (config);
}

// thread[n_filos] = monitor death
void	ft_clear_mem(t_philo *filo, pthread_mutex_t *forks, t_config *config,
	pthread_t *threads)
{
	int	i;

	i = 0;
	while (i <= config->n_filos)
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
	int	i;
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
	int	i;

	i = 0;
	while (i < config->n_filos)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
	pthread_mutex_destroy(&filo->config->permission_to_pick_forks);
}

void print_status(t_philo *philo, int status)
{
	printf("%ld %d is %s\n", get_current_time() - philo->config->start_time, \
		philo->id + 1, get_state_string(status));
}

const char *get_state_string(int state)
{
	if (state == THINK)
		return ("thinking");
	else if (state == EAT)
		return ("eating");
	else if (state == SLEEP)
		return ("sleeping");
	else if (state == DEAD)
		return ("dead");
	else if (state == PERM)
		return ("trying to lock permission_to_pick_forks");
	else if (state == NOPERM)
		return ("not allowed to lock permission_to_pick_forks");
	else if (state == PERM2)
		return ("locking permission_to_pick_forks");
	else if (state == LFORK)
		return ("grabbing the left fork");
	else if (state == RFORK)
		return ("grabbing the right fork");
	else if (state == DLFORK)
		return ("dropping the left fork");
	else if (state == DRFORK)
		return ("dropping the right fork");
	else
		return ("unknown");
}

void	*monitor_death(void *data)
{
	t_philo	*filo;
	int		i;
	int		n_filos;

	filo = (t_philo *)data;
	n_filos = filo[0].config->n_filos;
	while (1)
	{
		i = 0;
		while (i < n_filos)
		{
			if (is_philosopher_dead(&filo[i]))
				exit(1);
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}

int	is_philosopher_dead(t_philo *philo)
{
	if ((get_current_time() - philo->last_meal_t) > philo->config->t_to_die)
	{
		print_status(philo, DEAD);
		return (1);
	}
	return (0);
}
