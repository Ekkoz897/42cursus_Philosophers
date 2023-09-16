/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apereira <apereira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 10:02:26 by apereira          #+#    #+#             */
/*   Updated: 2023/09/16 13:26:10 by apereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

t_philo	*init_philo(char **av, t_philo *philo)
{
	philo = malloc(sizeof(t_philo));
	if (!philo)
		return (NULL);
	philo->t_to_die = ft_atoi(av[2]);
	philo->t_to_eat = ft_atoi(av[3]);
	philo->nbr_of_meals = ft_atoi(av[4]);
	return (philo);
}

int	main(int ac, char **av)
{
	t_philo	*philo;

	if (ac < 5 || ac > 6)
	{
		printf("Error: Wrong number of arguments\n");
		return (1);
	}
	philo = NULL;
	philo = init_philo(av, philo);
	if (!philo)
		return (1);
	printf("t_to_die: %i\n", philo->t_to_die);
	// if (start_simulation(philo))
	// 	return (1);
	return (0);
}
