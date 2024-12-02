/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzeggaf <mzeggaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 21:06:04 by mzeggaf           #+#    #+#             */
/*   Updated: 2024/12/02 19:49:40 by mzeggaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	get_len_of_lines(t_env *env)
{
	char	*line;
	int		count;
	int		fd;

	count = 0;
	fd = open(env->map.name, O_RDONLY);
	if (fd == -1)
		ft_exit(1, "parse: error opening file\n");
	while (1)
	{
		line = get_next_line(fd);
		if (line == NULL)
		{
			close(fd);
			return (count);
		}
		free(line);
		count++;
	}
	close(fd);
}

void	trim_lines(char **lines)
{
	char	*tmp;
	int		i;

	i = 0;
	while (lines[i])
	{
		tmp = lines[i];
		lines[i] = ft_strtrim(lines[i], "\n");
		free(tmp);
		if (lines[i] == NULL)
			ft_exit(0, "trim_lines: insufficient memory\n");
		i++;
	}
}

char	**get_lines(t_env *env)
{
	int		number_of_lines;
	char	**lines;
	int		fd;
	int		i;

	number_of_lines = get_len_of_lines(env);
	lines = malloc((number_of_lines + 1) * sizeof(char *));
	if (!lines)
		ft_exit(0, "parse: insufficient memory\n");
	fd = open(env->map.name, O_RDONLY);
	i = 0;
	while (i <= number_of_lines)
	{
		lines[i] = get_next_line(fd);
		i++;
	}
	close(fd);
	if (!lines[0])
		ft_exit(0, "empty map.\n");
	return (lines);
}

void	trim_empty_lines(char **lines)
{
	int	i;

	i = arr_len(lines) - 1;
	while (1)
	{
		if (lines[i][0] == '\0')
		{
			free(lines[i]);
			lines[i] = NULL;
		}
		else
			return ;
		i--;
	}
}

int	parse_cfcolor(t_env *env, char *line)
{
	char	**parts;
	char	**rgb;
	int		color;

	parts = ft_split(line, ' ');
	if (parts == NULL)
		ft_exit(0, "insufficient memory\n");
	if (arr_len(parts) != 2)
		ft_exit(0, "syntax error\n");
	rgb = ft_split(parts[1], ',');
	if (rgb == NULL)
		return (free_arr(parts));
	if (arr_len(rgb) != 3 || check_rgb_string(rgb))
		return (free_arr(parts));
	color = create_trgb(0x00, ft_atoi(rgb[0]), \
		ft_atoi(rgb[1]), ft_atoi(rgb[2]));
	if (!ft_strncmp(parts[0], "C", 1))
		env->ceiling_color = color;
	else
		env->floor_color = color;
	free_arr(parts);
	free_arr(rgb);
	return (0);
}
