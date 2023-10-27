/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apereira <apereira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 10:02:26 by apereira          #+#    #+#             */
/*   Updated: 2023/10/27 15:22:13 by apereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

t_philo	*init_philosophers(t_config *config, pthread_mutex_t *forks)
{
	int		i;
	t_philo	*filo;

	filo = malloc(config->n_filos * sizeof(t_philo));
	if (!filo)
		return (NULL);
	i = 0;
	while (i < config->n_filos)
	{
		filo[i].id = i;
		filo[i].meals_eaten = 0;
		filo[i].last_meal_t = get_current_time();
		filo[i].config = config;
		filo[i].left_fork = &forks[i];
		filo[i].right_fork = &forks[(i + 1) % config->n_filos];
		i++;
	}
	return (filo);
}

// Implementation of the philosopher's behavior
// usleep funciona em microsegundos, logo, 1000 = 1ms
// para evitar deadlock, fazemos com q o ultimo pegue no garfo da direita
// e todos devem pedir autorizacao para pegar nos garfos
void	*ft_filo(void *data)
{
	t_philo	*filo;
	long	min_last_meal_t;
	int		i;
	int		n_loops;

	filo = (t_philo *)data;
	n_loops = 0;
	while (n_loops < filo->config->max_meals)
	{
		print_status(filo, THINK);
		while (1)
		{
			print_status(filo, PERM);
			min_last_meal_t = filo->last_meal_t;
			if (pthread_mutex_lock(&filo->config->permission_to_pick_forks) != 0)
			{
				print_status(filo, NOPERM);
				usleep(1 * 1000);
				return (NULL);
			}
			i = 0;
			while (i < filo->config->n_filos)
			{
				if (filo[i].last_meal_t < min_last_meal_t)
					min_last_meal_t = filo[i].last_meal_t;
				i++;
			}
			if (filo->last_meal_t == min_last_meal_t)
				break ;
			else
			{
				print_status(filo, NOPERM);
				pthread_mutex_unlock(&filo->config->permission_to_pick_forks);
				usleep(1 * 1000);
				return (NULL);
			}
		}
		print_status(filo, PERM2);

		if (filo->id % 2 == 0)
			ft_pick_right_fork(filo);
		else
		{
			if (pthread_mutex_lock(filo->left_fork) != 0)
			{
				pthread_mutex_unlock(&filo->config->permission_to_pick_forks);
				usleep(1 * 1000);
				return (NULL);
			}
			print_status(filo, LFORK);

			if (pthread_mutex_lock(filo->right_fork) != 0)
			{
				pthread_mutex_unlock(filo->left_fork);
				print_status(filo, DLFORK);
				pthread_mutex_unlock(&filo->config->permission_to_pick_forks);
				usleep(1 * 1000);
				return (NULL);
			}
			print_status(filo, RFORK);
		}
		filo->last_meal_t = get_current_time();
		print_status(filo, EAT);
		usleep(filo->config->t_to_eat * 1000);

		pthread_mutex_unlock(filo->left_fork);
		print_status(filo, DLFORK);

		pthread_mutex_unlock(filo->right_fork);
		print_status(filo, DRFORK);

		print_status(filo, DEAD);
		pthread_mutex_unlock(&filo->config->permission_to_pick_forks);

		print_status(filo, SLEEP);
		usleep(filo->config->t_to_sleep * 1000);
		n_loops++;
	}
	return (NULL);
}

void	ft_start_simulation(t_philo *filo, pthread_mutex_t *forks,
		pthread_t *threads)
{
	int			i;

	i = 0;
	while (i < filo->config->n_filos)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&filo->config->permission_to_pick_forks, NULL);
	i = 0;
	while (i < filo->config->n_filos)
	{
		pthread_create(&threads[i], NULL, &ft_filo, &filo[i]);
		i++;
	}
	pthread_create(&threads[i], NULL, &monitor_death, filo);
}

// pthread_join espera que as threads terminem
int	main(int ac, char **av)
{
	t_philo			*filo;
	t_config		*config;
	pthread_mutex_t	*forks;
	pthread_t		*threads;

	config = init_vars(ac, av);
	if (!config)
		return (0);
	forks = malloc(ft_atoi(av[1]) * sizeof(pthread_mutex_t));
	threads = malloc((ft_atoi(av[1]) + 1) * sizeof(pthread_t));
	filo = init_philosophers(config, forks);
	if (!forks || !threads || !filo)
		return (0);
	ft_start_simulation(filo, forks, threads);
	mutex_destroyer(forks, filo, config);
	ft_clear_mem(filo, forks, config, threads);
	return (0);
}
