/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apereira <apereira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 13:40:10 by apereira          #+#    #+#             */
/*   Updated: 2023/10/16 14:47:53 by apereira         ###   ########.fr       */
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

typedef struct s_config
{
	int	n_filos;
	int	t_to_die;
	int	t_to_eat;
	int	t_to_sleep;
	int	max_meals;
	pthread_mutex_t permission_to_pick_forks;
}	t_config;

typedef struct s_philo
{
	int				id;
	int				state;
	int				meals_eaten;
	int				last_meal;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_config		*config;
}	t_philo;

// main.c
t_philo		*init_philo(char **av, t_philo *philo);
int			main(int nbr_of_philo, char **argv);

//utils.c
long		get_current_time(void);
long		ft_atoi(const char *str);
void		ft_pick_right_fork(t_philo *filo, void *data);
t_config	*init_vars(int ac, char **av);
void 		ft_clear_mem(t_philo *filo, pthread_mutex_t *forks, t_config *config,
				pthread_t *threads);
int			args_ok(int ac, char **av);
void 		mutex_destroyer(pthread_mutex_t *forks, t_philo *filo, t_config *config);
#endif
