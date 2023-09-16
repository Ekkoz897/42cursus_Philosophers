/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apereira <apereira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 13:40:10 by apereira          #+#    #+#             */
/*   Updated: 2023/09/16 13:40:50 by apereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <stdio.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <signal.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

# define DEAD 0
# define EAT 1
# define SLEEP 2
# define THINK 3

typedef struct s_filo
{
	int	id;
	int	state;
	int	meals_eaten;
	int	last_meal;
}t_filo;

typedef struct s_vars
{
	int		fd[2];
	int		pipe_fd[2];
	int		pid1;
	int		fd1;
	int		fd2;
	char	*temp;
	int		t_to_die;
	int		t_to_eat;
	int		nbr_of_meals;
	t_filo	*filo;
}t_philo;

// main.c
t_philo	*init_philo(char **av, t_philo *philo);
int		main(int nbr_of_philo, char **argv);

//utils.c
long	ft_atoi(const char *str);
#endif
