/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apereira <apereira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 10:02:26 by apereira          #+#    #+#             */
/*   Updated: 2023/11/11 14:00:57 by apereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

// Se houver apenas 1 filo, ele pega num garfo
// e acaba por morrer. num_filos = nbr of forks.
void	*filo(void *filo)
{
	t_filo	*filo_cpy;

	filo_cpy = (t_filo *)filo;
	if (filo_cpy->config->num_filos == 1)
	{
		print_status(filo, FORK);
		sleep_time(filo_cpy->config->t_to_die);
		return (NULL);
	}
	if (filo_cpy->id % 2 != 0)
		sleep_time(30);
	return (routines_loop(filo_cpy));
}

void	*routines_loop(t_filo *filo)
{
	while (1)
	{
		if (check_condition(filo) == 1)
			return (NULL);
		if (eating(filo) == 1)
			continue ;
		if (sleeping(filo) == 1)
			return (NULL);
		if (thinking(filo) == 1)
			return (NULL);
	}
}

// pthread_join espera que as threads terminem
int	main(int ac, char **av)
{
	int			i;
	pthread_t	*threads;
	pthread_t	police;
	t_config	*config;

	config = init(ac, av);
	if (!config)
		return (1);
	threads = malloc(sizeof(pthread_t) * (ft_atoi(av[1]) + 1));
	if (!threads)
		return (1);
	if (pthread_create(&police, NULL, &policia, config) != 0)
		return (exit_sim(NULL, police, config));
	i = 0;
	while (i < ft_atoi(av[1]))
	{
		if (pthread_create(&threads[i], NULL, &filo, config->filos[i]) != 0)
			return (exit_sim(threads, police, config));
		i++;
	}
	if (pthread_join(police, NULL) != 0)
		return (exit_sim(threads, police, config));
	destroy_sim(config, threads, config->forks);
	return (0);
}
