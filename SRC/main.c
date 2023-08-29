/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apereira <apereira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 10:02:26 by apereira          #+#    #+#             */
/*   Updated: 2023/08/29 10:09:08 by apereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	main(int nbr_of_philo, int t_to_die, int t_to_eat, int nbr_of_meals)
{
	t_philo	*philo;

	philo = init_philo(nbr_of_philo, t_to_die, t_to_eat, nbr_of_meals);
	if (!philo)
		return (1);
	if (start_simulation(philo))
		return (1);
	return (0);
}
