/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apereira <apereira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 10:21:20 by apereira          #+#    #+#             */
/*   Updated: 2023/11/11 13:04:42 by apereira         ###   ########.fr       */
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

int	validator(int argc, char **argv)
{
	int	meals;

	if (argc != 5 && argc != 6)
		return (-2);
	if (check_if_only_numbers(argv) == EXIT_FAILURE)
		return (-2);
	if (argc == 6)
	{
		meals = ft_atoi(argv[5]);
		if (meals <= 0)
			return (-2);
	}
	else
		meals = -1;
	return (meals);
}

int	check_if_only_numbers(char **argv)
{
	int	i;
	int	f;

	i = 1;
	while (argv[i])
	{
		if (ft_atoi(argv[i]) < 0)
			return (1);
		f = 0;
		while (argv[i][f])
		{
			if (argv[i][f] < 48 || argv[i][f] > 57)
				return (1);
			f++;
		}
		i++;
	}
	return (0);
}

// struct timevall guarda o tempo em segundos e microsegundos
long	get_current_time(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}

void	print_status(t_filo *filo, int action)
{
	time_t	time;

	time = get_current_time() - filo->config->start_time;
	pthread_mutex_lock(&filo->config->printing);
	if (action == 4)
		printf("%ld %d died\n", time, filo->id);
	if (check_if_dead(filo) == 1)
	{
		pthread_mutex_unlock(&filo->config->printing);
		return ;
	}
	if (action == 1)
		printf("%ld %d is eating\n", time, filo->id);
	else if (action == 2)
		printf("%ld %d is sleeping\n", time, filo->id);
	else if (action == 3)
		printf("%ld %d is thinking\n", time, filo->id);
	else if (action == 5)
		printf("%ld %d has grabbed a fork\n", time, filo->id);
	pthread_mutex_unlock(&filo->config->printing);
}
