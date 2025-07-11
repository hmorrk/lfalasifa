/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houarrak <houarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 19:40:50 by houarrak          #+#    #+#             */
/*   Updated: 2025/07/08 15:07:52 by houarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_simulation_end(t_rules *rules)
{
	int	end;

	pthread_mutex_lock(&rules->end_mutex);
	end = rules->simulation_end;
	pthread_mutex_unlock(&rules->end_mutex);
	return (end);
}

static int	check_philo_death(t_rules *rules)
{
	int		i;
	long	now;

	i = 0;
	while (i < rules->nb_philo)
	{
		pthread_mutex_lock(&rules->meal_mutex);
		now = get_time_ms();
		if (now - rules->philos[i].last_meal > rules->time_to_die)
		{
			pthread_mutex_lock(&rules->end_mutex);
			rules->simulation_end = 1;
			pthread_mutex_unlock(&rules->end_mutex);
			print_status(&rules->philos[i], "died");
			pthread_mutex_unlock(&rules->meal_mutex);
			return (1);
		}
		pthread_mutex_unlock(&rules->meal_mutex);
		i++;
	}
	return (0);
}

static int	check_all_full(t_rules *rules)
{
	int	full;
	int	i;

	if (rules->nb_meals_required <= 0)
		return (0);
	full = 0;
	i = 0;
	while (i < rules->nb_philo)
	{
		pthread_mutex_lock(&rules->meal_mutex);
		if (rules->philos[i].meals_eaten >= rules->nb_meals_required)
			full++;
		pthread_mutex_unlock(&rules->meal_mutex);
		i++;
	}
	if (full == rules->nb_philo)
	{
		pthread_mutex_lock(&rules->end_mutex);
		rules->simulation_end = 1;
		pthread_mutex_unlock(&rules->end_mutex);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_rules	*rules;

	rules = (t_rules *)arg;
	while (!is_simulation_end(rules))
	{
		if (check_philo_death(rules))
			return (NULL);
		if (check_all_full(rules))
			return (NULL);
		usleep(500);
	}
	return (NULL);
}
