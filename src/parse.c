/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzeggaf <mzeggaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 19:41:06 by mzeggaf           #+#    #+#             */
/*   Updated: 2024/12/02 19:46:39 by mzeggaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// TODO: when only the map are provided it is a segfault
// TODO: when the file do not exist it segfaults
// TODO: player position is not working (NWSE)

static char	parse_character(char c, t_env *env, int i, int h)
{
	if (ft_index(c, "NESW") < 4)
	{
		env->player.x = i + 0.5;
		env->player.y = h + 0.5;
		env->player.angle = ft_index(c, "NESW");
		return ('0');
	}
	else if (c == 'F')
	{
		env->sp.x = i + 0.5;
		env->sp.y = h + 0.5;
		return ('0');
	}
	else
		return (c);
}

static char	*parse_line(char *line, t_env *env, int h)
{
	t_map	*map;
	char	*blocks;
	int		i;

	map = &env->map;
	blocks = (char *)malloc((map->width + 1) * sizeof(char));
	if (!blocks)
		ft_exit(3, "error allocating memory.\n");
	i = 0;
	while (i < map->width)
	{
		if (*line)
		{
			blocks[i] = parse_character(*line, env, i, h);
			line++;
		}
		else
			blocks[i] = '1';
		i++;
	}
	return (blocks);
}

static void	init_map_data(t_env *env, char **lines)
{
	char	*line;
	int		h;

	h = 0;
	env->map.height = arr_len(lines);
	env->map.width = get_map_width(lines);
	env->map.blocks = (char **)malloc(env->map.height * sizeof(char *));
	if (!env->map.blocks)
		ft_exit(3, "error allocating memory.\n");
	while (1)
	{
		line = lines[h];
		if (!line)
			break ;
		env->map.blocks[h] = parse_line(line, env, h);
		h++;
	}
}

static int	parse_map_info(char **lines, t_env *env)
{
	char	*line;
	int		i;

	i = 0;
	while (lines[i])
	{
		line = ft_strtrim(lines[i], " \n");
		if (!ft_strncmp(line, "NO ", 3))
			env->no_path = ft_strtrim(&line[3], " ");
		else if (!ft_strncmp(line, "SO ", 3))
			env->so_path = ft_strtrim(&line[3], " ");
		else if (!ft_strncmp(line, "WE ", 3))
			env->we_path = ft_strtrim(&line[3], " ");
		else if (!ft_strncmp(line, "EA ", 3))
			env->ea_path = ft_strtrim(&line[3], " ");
		else if (!ft_strncmp(line, "F ", 2) || !ft_strncmp(line, "C ", 2))
			parse_cfcolor(env, line);
		else if (line[0] != '\0')
			break ;
		free(line);
		line = NULL;
		i++;
	}
	free(line);
	return (i);
}

void	parse_params(t_env *env)
{
	char	**lines;
	int		i;
	int		j;
	int		h;

	lines = get_lines(env);
	trim_lines(lines);
	i = parse_map_info(lines, env);
	check_if_start_of_map(lines[i]);
	trim_empty_lines(lines + i);
	check_map_lines(lines + i);
	init_map_data(env, lines + i);
	j = 0;
	while (j < env->map.height)
	{
		h = 0;
		while (h < env->map.width)
		{
			if (env->map.blocks[j][h] == '0')
				flood_fill(env, lines + i, h, j);
			h++;
		}
		j++;
	}
	free_arr(lines);
}
