/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houarrak <houarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 19:32:10 by houarrak          #+#    #+#             */
/*   Updated: 2025/07/08 14:47:16 by houarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <string.h>

static int	is_simulation_end(t_philo *philo)
{
	int	end;

	pthread_mutex_lock(&philo->rules->end_mutex);
	end = philo->rules->simulation_end;
	pthread_mutex_unlock(&philo->rules->end_mutex);
	return (end);
}

static void	handle_one_philo(t_philo *philo)
{
	t_rules	*rules;

	rules = philo->rules;
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, "has taken a fork");
	ft_usleep(rules->time_to_die);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_lock(&rules->end_mutex);
	rules->simulation_end = 1;
	pthread_mutex_unlock(&rules->end_mutex);
	print_status(philo, "died");
}

static void	philo_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->rules->meal_mutex);
	philo->last_meal = get_time_ms();
	print_status(philo, "is eating");
	pthread_mutex_unlock(&philo->rules->meal_mutex);
	ft_usleep(philo->rules->time_to_eat);
	pthread_mutex_lock(&philo->rules->meal_mutex);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->rules->meal_mutex);
}

static void	philo_sleep_and_think(t_philo *philo)
{
	print_status(philo, "is sleeping");
	ft_usleep(philo->rules->time_to_sleep);
	print_status(philo, "is thinking");
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->rules->nb_philo == 1)
	{
		handle_one_philo(philo);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		usleep(1000);
	while (!is_simulation_end(philo))
	{
		lock_forks(philo);
		philo_eat(philo);
		unlock_forks(philo);
		philo_sleep_and_think(philo);
	}
	return (NULL);
}
