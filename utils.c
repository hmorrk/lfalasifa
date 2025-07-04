/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houarrak <houarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 19:19:17 by houarrak          #+#    #+#             */
/*   Updated: 2025/07/03 20:24:26 by houarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	error_msg(const char *msg)
{
	printf("Error: %s\n", msg);
	return (1);
}

static int	ft_isdigit(char c)
{
	return (c >= '0' && c <= '9');
}

static const char	*skip_space_sign(const char *str)
{
	int	i;

	i = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			return (NULL);
		i++;
	}
	return (str + i);
}

long	ft_atol(const char *str)
{
	long	res;
	int		len;

	res = 0;
	len = 0;
	str = skip_space_sign(str);
	if (!str || !ft_isdigit(*str))
		return (-1);
	while (ft_isdigit(*str))
	{
		res = res * 10 + (*str - '0');
		str++;
		len++;
	}
    if (*str != '\0')
		return (-1);
	if (len > 10 || res > INT_MAX)
		return (-1);
	return (res);
}

long	get_time_ms(void)
{
	struct timeval	tv;
	long			ms;

	gettimeofday(&tv, NULL);
	ms = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (ms);
}


void ft_usleep(long time_in_ms)
{
	long	start;

	start = get_time_ms();
	while ((get_time_ms() - start) < time_in_ms)
		usleep(500);
}