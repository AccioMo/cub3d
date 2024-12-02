/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzeggaf <mzeggaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 15:52:19 by mzeggaf           #+#    #+#             */
/*   Updated: 2024/12/02 19:04:16 by mzeggaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	ft_index(char c, char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (c == str[i])
			return (i);
		i++;
	}
	return (i);
}

int	arr_len(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		i++;
	return (i);
}

double	ft_get_time(void)
{
	struct timeval	counter;

	gettimeofday(&counter, NULL);
	return (counter.tv_sec * 1000.0 + counter.tv_usec / 1000.0);
}

int	create_trgb(int t, int r, int g, int b)
{
	return (t << 24 | r << 16 | g << 8 | b);
}

int	get_map_width(char **lines)
{
	int	i;
	int	result;
	int	height;

	i = 0;
	result = 0;
	height = arr_len(lines);
	while (i < height)
	{
		if (result < (int) ft_strlen(lines[i]))
			result = ft_strlen(lines[i]);
		i++;
	}
	return (result);
}
