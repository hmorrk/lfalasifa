/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houarrak <houarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 19:11:44 by houarrak          #+#    #+#             */
/*   Updated: 2025/07/03 20:06:28 by houarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/time.h>
#include <limits.h>

// ./philo 5 800 200 100 [5]

typedef struct s_philo t_philo;
typedef struct s_rules t_rules;

struct s_philo
{
	int				id;
	long			last_meal;
	int				meals_eaten;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_t		thread;
	t_rules			*rules;
    int             fork_order;
};

struct s_rules
{
	int				nb_philo;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				nb_meals_required;
	int				simulation_end;
	long			start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	meal_mutex;
	pthread_mutex_t	end_mutex;
	t_philo			*philos;
};

// UTILS
long	ft_atol(const char *str);
long get_time_ms(void);
void ft_usleep(long time_in_ms);


void	print_status(t_philo *philo, char *msg);
void	ft_usleep(long ms);
int		parse_args(t_rules *rules, int ac, char **av);
int		init_philos_and_forks(t_rules *rules);
void	*philo_routine(void *args);
void	*monitor_routine(void *arg);

#endif