/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houarrak <houarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 19:32:10 by houarrak          #+#    #+#             */
/*   Updated: 2025/07/04 02:29:03 by houarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <string.h>

static int	is_died(const char *msg)
{
	return (msg[0] == 'd' && msg[1] == 'i' && msg[2] == 'e'
		&& msg[3] == 'd' && msg[4] == '\0');
}

void	print_status(t_philo *philo, char *msg)
{
	long	timestamp;

	pthread_mutex_lock(&philo->rules->print_mutex);
	pthread_mutex_lock(&philo->rules->end_mutex);
	if (!philo->rules->simulation_end || is_died(msg))
	{
		timestamp = get_time_ms() - philo->rules->start_time;
		printf("%ld %d %s\n", timestamp, philo->id, msg);
	}
	pthread_mutex_unlock(&philo->rules->end_mutex);
	pthread_mutex_unlock(&philo->rules->print_mutex);
}

void	lock_forks(t_philo *philo)
{
	if (philo->fork_order == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
	}
}
void	unlock_forks(t_philo *philo)
{
    pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_unlock(philo->right_fork);
}

static int	is_simulation_end(t_philo *philo)
{
	int	end;
	pthread_mutex_lock(&philo->rules->end_mutex);
	end = philo->rules->simulation_end;
	pthread_mutex_unlock(&philo->rules->end_mutex);
	return (end);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo = (t_philo *)arg;
	t_rules *rules = philo->rules;

	if (rules->nb_philo == 1)
	{
		print_status(&rules->philos[0], "has taken a fork");
		ft_usleep(rules->time_to_die);
		pthread_mutex_lock(&rules->end_mutex);
		rules->simulation_end = 1;
		pthread_mutex_unlock(&rules->end_mutex);
		print_status(&rules->philos[0], "died");
		return (NULL);
	}

	if (philo->id % 2 == 0)
		usleep(1000);

	while (!is_simulation_end(philo))
	{

		lock_forks(philo);

		pthread_mutex_lock(&philo->rules->meal_mutex);
		philo->last_meal = get_time_ms();
		print_status(philo, "is eating");
		pthread_mutex_unlock(&philo->rules->meal_mutex);

		ft_usleep(philo->rules->time_to_eat);

		pthread_mutex_lock(&philo->rules->meal_mutex);
		philo->meals_eaten++;
		pthread_mutex_unlock(&philo->rules->meal_mutex);

		unlock_forks(philo);

		print_status(philo, "is sleeping");
		ft_usleep(philo->rules->time_to_sleep);

		print_status(philo, "is thinking");
	}
	return (NULL);
}
