/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apereira <apereira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 13:40:10 by apereira          #+#    #+#             */
/*   Updated: 2023/11/11 13:52:37 by apereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>
# include <pthread.h>
# include <stdbool.h>

# define EATING 1
# define SLEEPING 2
# define THINKING 3
# define DEAD 4
# define FORK 5

typedef struct s_config	t_config;

// meal mutex -> locks access to the time of last meal each philo has had
// number mutex -> locks access to the number of meals each philo has had
typedef struct s_filo
{
	int				id;
	pthread_mutex_t	meal;
	long			last_meal;
	pthread_mutex_t	number;
	int				meal_nbr;
	t_config		*config;
}	t_filo;

typedef struct s_config
{
	time_t			start_time;
	int				num_filos;
	int				t_to_die;
	int				t_to_eat;
	int				t_to_sleep;
	int				max_meals;
	t_filo			**filos;
	int				*frk;
	pthread_mutex_t	*forks;
	pthread_mutex_t	life;
	pthread_mutex_t	printing;
	pthread_mutex_t	*searching;
	int				dead;
	int				full;
}	t_config;

// main.c
void		*filo(void *filo);
void		*routines_loop(t_filo *filo);
int			main(int ac, char **argv);

/*Init.c*/
t_config	*init(int ac, char **av);
t_config	*init_values(t_config *config, int ac, char **av);
t_config	*init_filos(t_config *config, int num_filos);
void		init_mutexes(t_config *config);
t_config	*allocate_mut(t_config *config, int num_filos);

/*Utils.c*/
long		ft_atoi(const char *str);
int			validator(int argc, char **argv);
int			check_if_only_numbers(char **argv);
long		get_current_time(void);
void		print_status(t_filo *filo, int action);

/*Utils2.c*/
int			exit_sim(pthread_t *threads, pthread_t police, t_config *config);
void		destroy_sim(t_config *config, pthread_t *threads, \
				pthread_mutex_t *frk);
void		destroy_mutexes(t_config *config, pthread_mutex_t *forks);
void		free_config(t_config *config, int num_philo);
void		sleep_time(int time_to_sleep);

/*Utils3.c*/
int			*get_index_f(t_filo *filo);
int			locking_forks(t_filo *filo, int *forks);
void		unlock_forks(t_filo *filo, int *forks);
void		meal_time(t_filo *filo);
void		increase_meal(t_filo *filo);

/*Police.c*/
void		*policia(void *config);
int			check_all_max_meals(t_config *config);
int			check_max_meals(t_filo *filo);
int			check_death(t_filo *filo);
int			check_if_dead(t_filo *filo);

/*Routines.c*/
int			check_condition(t_filo *filo);
int			eating(t_filo *filo);
int			sleeping(t_filo *filo);
int			thinking(t_filo *filo);

#endif
