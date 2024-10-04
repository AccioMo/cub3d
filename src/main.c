/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzeggaf <mzeggaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 19:59:25 by mzeggaf           #+#    #+#             */
/*   Updated: 2024/10/04 17:13:35 by mzeggaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// char	**read_map(char *filename)
// {
// 	int		fd;
// 	char	*line;
// 	char	**map;

// 	fd = open(filename, O_RDONLY);
// 	if (fd < 0)
// 	{
// 		ft_putstr_fd("error opening file.\n", 2);
// 		return (NULL);
// 	}
// 	while (true)
// 	{
// 		line = get_next_line(fd);
// 		if (!line)
// 			break ;
// 		free(line);
// 		if (!map)
// 			break ;
// 	}
// }

void	ft_exit(int code, char *msg)
{
	ft_putstr_fd("Error\n", 2);
	if (code == 1)
		ft_putstr_fd("Error opening file: ", 2);
	else if (code == 2)
		ft_putstr_fd("Invalid file extension: ", 2);
	else if (code == 3)
		ft_putstr_fd("Error allocating memory: ", 2);
	else if (code == 4)
		ft_putstr_fd("Error reading file: ", 2);
	else if (code == 5)
		ft_putstr_fd("mlx: ", 2);
	ft_putstr_fd(msg, 2);
	exit(EXIT_FAILURE);
}

void	init_mlx(t_env *env)
{
	env->mlx = mlx_init();
	if (!env->mlx)
		ft_exit(5, "init error.\n");
	env->win = mlx_new_window(env->mlx, W_WIDTH, W_HEIGHT, "Fil de Fer");
	if (!env->win)
		ft_exit(5, "error creating window.\n");
	env->img.img = mlx_new_image(env->mlx, W_WIDTH, W_HEIGHT);
	if (!env->img.img)
		ft_exit(5, "error creating image.\n");
	env->img.addr = mlx_get_data_addr(env->img.img, &env->img.bpp, \
		&env->img.n_bytes, &env->img.endian);
	if (!env->img.addr)
		ft_exit(5, "error initializing image data.\n");
}

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

void	ft_put_pixel(int x, int y, int color, t_image *img)
{
	char	*dst;

	if ((0 <= x && x < W_WIDTH) && (0 <= y && y < W_HEIGHT))
	{
		x *= (img->bpp / 8);
		y *= img->n_bytes;
		dst = img->addr + x + y;
		*(unsigned int *) dst = color;
	}
}

void	ft_paint_block(t_block *block, t_env *env)
{
	int	h;
	int	w;

	h = 0;
	while (h < block->size)
	{
		w = 0;
		while (w < block->size)
		{
			ft_put_pixel(w + block->x, h + block->y, 0xFFFFFF * block->wall, &env->img);
			w += 1;
		}
		h += 1;
	}
}

void	draw_map(t_env *env)
{
	t_map *map = &env->map;
	int h = 0;
	int w = 0;

	while (h < map->height)
	{
		w = 0;
		while (w < map->width)
		{
			ft_paint_block(&map->blocks[h][w], env);
			w += 1;
		}
		h += 1;
	}
}

void	draw_player(t_env *env)
{
	double i = 0;
	double angle;
	double x1;
	double y1;
	double r = 10;

	while (i < 360)
	{
		angle = i;
		x1 = r * cos(angle * M_PI / 180);
		y1 = r * sin(angle * M_PI / 180);
		ft_put_pixel(env->player.x + x1, env->player.y + y1, 0xFFFFFF, &env->img);
		i += 0.1;
	}
}

t_block	*parse_line(char *line, t_env *env, int h)
{
	t_map	*map = &env->map;
	t_block	*blocks;
	int	i;
	if (map->width == 0)
		map->width = ft_strlen(line);
	else if (map->width != (int)ft_strlen(line))
		ft_exit(5, "invalid map.\n");
	blocks = (t_block *)malloc((map->width + 1) * sizeof(t_block));
	i = 0;
	while (i < map->width)
	{
		blocks[i].x = i * BLOCK_SIZE;
		blocks[i].y = h * BLOCK_SIZE;
		blocks[i].size = BLOCK_SIZE;
		if (line[i] == '1')
			blocks[i].wall = 1;
		else if (line[i] == '0')
			blocks[i].wall = 0;
		else if (ft_index(line[i], "NESW") < 4)
		{
			env->player.x = blocks[i].x + BLOCK_SIZE / 2;
			env->player.y = blocks[i].y + BLOCK_SIZE / 2;
			env->player.angle = ft_index(line[i], "NESW");
		}
		i++;
	}
	return (blocks);
}

void	init_map(t_env *env)
{
	char	*line;
	int		fd;
	int		h;

	env->map.width = 0;
	env->map.height = 0;
	fd = open(env->map.name, O_RDONLY);
	if (fd < 0)
		ft_exit(1, env->map.name);
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		free(line);
		env->map.height++;
	}
	close(fd);
	printf("height: %d\n", env->map.height);
	env->map.blocks = (t_block **)malloc(env->map.height * sizeof(t_block *));
	fd = open(env->map.name, O_RDONLY);
	if (fd < 0)
		ft_exit(1, env->map.name);
	h = 0;
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		env->map.blocks[h] = parse_line(line, env, h);
		free(line);
		h++;
	}
}

int main(int ac, char **av)
{
	t_env	env;

	if (ac != 2)
	{
		ft_putstr_fd("usage: ./cub3d [map].cub\n", 2);
		return (1);
	}
	env.map.name = av[1];
	init_map(&env);
	init_mlx(&env);
	draw_map(&env);
	draw_player(&env);
	
	mlx_put_image_to_window(env.mlx, env.win, env.img.img, 0, 0);

	mlx_loop(env.mlx);
}
