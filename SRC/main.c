/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apereira <apereira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 10:02:26 by apereira          #+#    #+#             */
/*   Updated: 2023/10/16 15:26:23 by apereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

t_philo *init_philosophers(t_config *config, pthread_mutex_t *forks)
{
	int	i;
	t_philo *filo;

	filo = malloc(config->n_filos * sizeof(t_philo));
	if (!filo)
		return (NULL);
	i = 0;
	while (i < config->n_filos)
	{
		filo[i].id = i;
		filo[i].state = THINK;
		filo[i].meals_eaten = 0;
		filo[i].last_meal = 0;
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
void	*ft_filo(void *data)
{
	t_philo	*filo;
	int		n_loops;

	filo = (t_philo *)data;
	n_loops = 0;
	while (n_loops < 5)
	{
		printf("P%d is thinking\n", filo->id);
	
		printf("P%d trying to lock permission_to_pick_forks\n", filo->id);
		pthread_mutex_lock(&filo->config->permission_to_pick_forks);
		printf("P%d locked permission_to_pick_forks\n", filo->id);

		if (filo->id == filo->config->n_filos - 1)
			ft_pick_right_fork(filo, data);
		else
		{
			printf("P%d grabbed the left fork\n", filo->id);
			pthread_mutex_lock(filo->left_fork);
			
			printf("P%d grabbed the right fork\n", filo->id);
			pthread_mutex_lock(filo->right_fork);
		}

		printf("P%d is eating\n", filo->id);
		filo->last_meal = get_current_time();
		usleep(filo->config->t_to_eat * 1000);
		
		printf("P%d dropped the left fork\n", filo->id);
		pthread_mutex_unlock(filo->left_fork);
		
		printf("P%d dropped the right fork\n", filo->id);
		pthread_mutex_unlock(filo->right_fork);
		
		pthread_mutex_unlock(&filo->config->permission_to_pick_forks);
		
		printf("P%d is sleeping\n", filo->id);
		usleep(filo->config->t_to_sleep * 1000);
		n_loops++;
	}
	return (NULL);
}

// philosophers[i].right_fork = &forks[(i + 1) % N] -> para ter
// certeza que o ultimo filosofo pega no primeiro garfo com a mao direita
void	ft_start_simulation(t_philo *filo, pthread_mutex_t *forks,
		pthread_t *threads)
{
	int	i;

	i = 0;
	while (i < filo->config->n_filos)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	i = 0;
	while (i < filo->config->n_filos)
	{
		pthread_create(&threads[i], NULL, &ft_filo, &filo[i]);
		i++;
	}
}

// pthread_join espera que as threads terminem
int	main(int ac, char **av)
{
	t_philo		*filo;
	t_config	*config;
	pthread_mutex_t	*forks;
	pthread_t		*threads;

	config = init_vars(ac, av);
	if(!config)
		return (0);
	forks = malloc(ft_atoi(av[1]) * sizeof(pthread_mutex_t));
	threads = malloc(ft_atoi(av[1]) * sizeof(pthread_t));
	filo = init_philosophers(config, forks);
	if (!forks || !threads || !filo)
		return (0);
	printf("Initializing permission_to_pick_forks mutex\n");
	pthread_mutex_init(&filo->config->permission_to_pick_forks, NULL);
	printf("Initialized permission_to_pick_forks mutex\n");
	ft_start_simulation(filo, forks, threads);
	mutex_destroyer(forks, filo, config);
	ft_clear_mem(filo, forks, config, threads);
	return (0);
}
