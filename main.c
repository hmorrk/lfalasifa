/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houarrak <houarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 19:16:48 by houarrak          #+#    #+#             */
/*   Updated: 2025/07/07 21:15:34 by houarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_all_mutex(t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->nb_philo)
		pthread_mutex_destroy(&rules->forks[i++]);
	pthread_mutex_destroy(&rules->print_mutex);
	pthread_mutex_destroy(&rules->meal_mutex);
	pthread_mutex_destroy(&rules->end_mutex);
}

int	print_error(char *msg)
{
	printf("%s\n", msg);
	return (1);
}

int	main(int ac, char **av)
{
	t_rules		rules;
	pthread_t	monitor;
	int			i;

	if (parse_args(&rules, ac, av) || init_philos_and_forks(&rules))
		return (print_error("Error: Invalid arguments or init failed"));
	rules.start_time = get_time_ms();
	i = -1;
	while (++i < rules.nb_philo)
	{
		rules.philos[i].last_meal = rules.start_time;
		if (pthread_create(&rules.philos[i].thread, NULL, philo_routine,
				&rules.philos[i]))
			return (print_error("Error: pthread_create"));
	}
	if (pthread_create(&monitor, NULL, monitor_routine, &rules))
		return (print_error("Error: monitor thread"));
	pthread_join(monitor, NULL);
	i = -1;
	while (++i < rules.nb_philo)
		pthread_join(rules.philos[i].thread, NULL);
	destroy_all_mutex(&rules);
	free(rules.forks);
	free(rules.philos);
	return (0);
}
