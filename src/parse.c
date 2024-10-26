/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzeggaf <mzeggaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 15:42:28 by mzeggaf           #+#    #+#             */
/*   Updated: 2024/10/23 16:15:07 by mzeggaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int *parse_line(char *line, t_env *env, int h)
{
	t_map	*map = &env->map;
	int		*blocks;
	int i;
	
	blocks = (int *)malloc((map->width + 1) * sizeof(int));
	if (!blocks)
		ft_exit(3, "error allocating memory.\n");
	i = 0;
	while (i < map->width)
	{
		if (line[i] == '1')
			blocks[i] = 1;
		else if (line[i] == '0')
			blocks[i] = 0;
		else if (ft_index(line[i], "NESW") < 4)
		{
			printf("player: %c\n", line[i]);
			env->player.x = BLOCK_SIZE * i;
			env->player.y = BLOCK_SIZE * h;
			env->player.angle = ft_index(line[i], "NESW");
			blocks[i] = 0;
		}
		else
			break;
		i++;
	}
	return (blocks);
}

static void	ft_map_check(char *line, t_env *env)
{
	int w;

	w = ft_strlen(line);
	w -= (line[w - 1] == '\n');
	if (env->map.width == 0)
		env->map.width = w;
	else if (w != env->map.width)
	{
		printf("width: %d\n", w);
		env->map.width = w;
		if (w == 1 && line[0] == '\n')
			return ;
		ft_exit(5, "map width inconsistent.\n");
	}
	env->map.width = w;
}

void init_map(t_env *env)
{
	char *line;
	int fd;
	int h;

	env->map.width = 0;
	env->map.height = 0;
	env->map.x_offset = 0;
	env->map.y_offset = 0;
	fd = open(env->map.name, O_RDONLY);
	if (fd < 0)
		ft_exit(1, env->map.name);
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break;
		ft_map_check(line, env);
		free(line);
		env->map.height++;
	}
	close(fd);
	if (env->map.height <= MINIMAP_BLOCKS)
		ft_exit(5, "minimap size incompatible with map height: change MINIMAP_BLOCKS.\n");
	else if (env->map.width <= MINIMAP_BLOCKS)
		ft_exit(5, "minimap size incompatible with map height: change MINIMAP_SIZE.\n");
	printf("height: %d\n", env->map.height);
	env->map.blocks = (int **)malloc(env->map.height * sizeof(int *));
	if (!env->map.blocks)
		ft_exit(3, "error allocating memory.\n");
	fd = open(env->map.name, O_RDONLY);
	if (fd < 0)
		ft_exit(1, env->map.name);
	h = 0;
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break;
		env->map.blocks[h] = parse_line(line, env, h);
		free(line);
		h++;
	}
}
