/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzeggaf <mzeggaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 20:58:33 by mzeggaf           #+#    #+#             */
/*   Updated: 2024/12/02 22:27:02 by mzeggaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	check_map_lines(char **lines)
{
	int	i;
	int	j;
	int	player;
	int	sprite;

	i = 0;
	player = 0;
	sprite = 0;
	while (lines[i])
	{
		j = 0;
		while (lines[i][j])
		{
			if (!ft_strchr("01 NSEWFD", lines[i][j]))
				ft_exit(0, "bad map\n");
			if (ft_strchr("NSEW", lines[i][j]))
				player++;
			if (lines[i][j] == 'F')
				sprite++;
			j++;
		}
		i++;
	}
	if (player != 1 || sprite > 1)
		ft_exit(0, "bad map\n");
}

int	check_rgb_string(char **rgb)
{
	int	i;
	int	j;

	i = 0;
	while (i < 3)
	{
		j = 0;
		while (rgb[i][j])
		{
			if (!ft_isdigit(rgb[i][j]) && !(rgb[i][j] == '+' && j == 0))
				return (1);
			j++;
		}
		if (ft_atoi(rgb[i]) > 0xff)
			return (1);
		i++;
	}
	return (0);
}

void	check_if_start_of_map(char *line)
{
	int	i;

	i = 0;
	if (!line)
		ft_exit(0, "unexpected line\n");
	while (line[i])
	{
		if (line[i] != '1' && line[i] != ' ')
		{
			if (line[i] != '0' && line[i] != 'N' && line[i] != 'S' && \
			line[i] != 'E' && line[i] != 'W' && line[i] != 'D' && \
			line[i] != 'F')
				ft_exit(0, "unexpected line\n");
		}
		i++;
	}
}
