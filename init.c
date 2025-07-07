/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houarrak <houarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 19:18:16 by houarrak          #+#    #+#             */
/*   Updated: 2025/07/07 21:15:15 by houarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	parse_args(t_rules *rules, int ac, char **av)
{
	if (ac != 5 && ac != 6)
		return (1);
	rules->nb_philo = ft_atol(av[1]);
	rules->time_to_die = ft_atol(av[2]);
	rules->time_to_eat = ft_atol(av[3]);
	rules->time_to_sleep = ft_atol(av[4]);
	if (ac == 6)
		rules->nb_meals_required = ft_atol(av[5]);
	else
		rules->nb_meals_required = 0;
	rules->simulation_end = 0;
	if (rules->nb_philo < 1 || rules->time_to_die < 0 || rules->time_to_eat < 0
		|| rules->time_to_sleep < 0 || (ac == 6
			&& rules->nb_meals_required < 1))
		return (1);
	return (0);
}

static int	init_mutexes(t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->nb_philo)
	{
		if (pthread_mutex_init(&rules->forks[i], NULL) != 0)
			return (1);
		i++;
	}
	if (pthread_mutex_init(&rules->print_mutex, NULL) != 0
		|| pthread_mutex_init(&rules->meal_mutex, NULL) != 0
		|| pthread_mutex_init(&rules->end_mutex, NULL) != 0)
		return (1);
	return (0);
}

static void	assign_forks_and_philo(t_rules *rules, int i)
{
	rules->philos[i].id = i + 1;
	rules->philos[i].meals_eaten = 0;
	rules->philos[i].last_meal = 0;
	rules->philos[i].rules = rules;
	rules->philos[i].left_fork = &rules->forks[i];
	if (i == rules->nb_philo - 1)
		rules->philos[i].right_fork = &rules->forks[0];
	else
		rules->philos[i].right_fork = &rules->forks[i + 1];
	if (i == rules->nb_philo - 1)
		rules->philos[i].fork_order = 1;
	else
		rules->philos[i].fork_order = 0;
}

int	init_philos_and_forks(t_rules *rules)
{
	int	i;

	rules->philos = malloc(sizeof(t_philo) * rules->nb_philo);
	if (!rules->philos)
		return (1);
	rules->forks = malloc(sizeof(pthread_mutex_t) * rules->nb_philo);
	if (!rules->forks)
	{
		free(rules->philos);
		return (1);
	}
	if (init_mutexes(rules))
	{
		free(rules->philos);
		free(rules->forks);
		return (1);
	}
	i = 0;
	while (i < rules->nb_philo)
	{
		assign_forks_and_philo(rules, i);
		i++;
	}
	return (0);
}
