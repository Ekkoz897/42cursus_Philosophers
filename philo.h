/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apereira <apereira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 13:40:10 by apereira          #+#    #+#             */
/*   Updated: 2023/10/02 13:35:16 by apereira         ###   ########.fr       */
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
	int		n_filos;
	int		t_to_die;
	int		t_to_eat;
	int		t_to_sleep;
	int				id;
	int				state;
	int				meals_eaten;
	int				last_meal;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
}t_filo;

typedef struct s_vars
{
	int		min_meals;
	t_filo	*filo;
}t_vars;

// main.c
t_vars	*init_philo(char **av, t_vars *philo);
int		main(int nbr_of_philo, char **argv);

//utils.c
long	get_current_time(void);
long	ft_atoi(const char *str);
void	ft_pick_right_fork(t_filo *filo,void *data);
#endif
