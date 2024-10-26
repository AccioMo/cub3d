/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzeggaf <mzeggaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 15:44:10 by mzeggaf           #+#    #+#             */
/*   Updated: 2024/10/25 13:30:46 by mzeggaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int ft_mlx_move(int keycode, t_env *env)
{
	double newx = env->player.x;
	double newy = env->player.y;

	if (keycode == KEY_A)
	{
		newx += env->dirVec.x * MOVE_SPEED;
		newy -= env->dirVec.y * MOVE_SPEED;
		if (env->map.blocks[(int)newy / BLOCK_SIZE][(int)newx / BLOCK_SIZE] == 0)
		{
			env->player.x = newx;
			env->player.y = newy;
		}
		refresh(env);
		return (0);
	}
	else if (keycode == KEY_D)
	{
		newx -= env->dirVec.x * MOVE_SPEED;
		newy += env->dirVec.y * MOVE_SPEED;
		if (env->map.blocks[(int)newy / BLOCK_SIZE][(int)newx / BLOCK_SIZE] == 0)
		{
			env->player.x = newx;
			env->player.y = newy;
		}
		refresh(env);
		return (0);
	}
	else if (keycode == KEY_W)
	{
		newx += round(env->dirVec.x * MOVE_SPEED);
		newy += round(env->dirVec.y * MOVE_SPEED);
		if (env->map.blocks[(int)newy / BLOCK_SIZE][(int)newx / BLOCK_SIZE] == 0)
		{
			env->player.x = newx;
			env->player.y = newy;
		}
		refresh(env);
		return (0);
	}
	else if (keycode == KEY_S)
	{
		newx -= round(env->dirVec.x * MOVE_SPEED);
		newy -= round(env->dirVec.y * MOVE_SPEED);
		if (env->map.blocks[(int)newy / BLOCK_SIZE][(int)newx / BLOCK_SIZE] == 0)
		{
			env->player.x = newx;
			env->player.y = newy;
		}
		refresh(env);
		return (0);
	}
	return (1);
}

int ft_key_event(int keycode, void *ptr)
{
	t_env *env;
	double rotSpeed;

	rotSpeed = 0.03;
	env = (t_env *)ptr;
	if (keycode == ESC)
		ft_end(env);
	else if (keycode == KEY_RIGHT)
	{
		double oldDirX = env->dirVec.x;
		env->dirVec.x = env->dirVec.x * cos(-rotSpeed) - env->dirVec.y * sin(-rotSpeed);
		env->dirVec.y = oldDirX * sin(-rotSpeed) + env->dirVec.y * cos(-rotSpeed);
		double oldPlaneX = env->planeVec.x;
		env->planeVec.x = env->planeVec.x * cos(-rotSpeed) - env->planeVec.y * sin(-rotSpeed);
		env->planeVec.y = oldPlaneX * sin(-rotSpeed) + env->planeVec.y * cos(-rotSpeed);
		refresh(env);
	}
	else if (keycode == KEY_LEFT)
	{
		double oldDirX = env->dirVec.x;
		env->dirVec.x = env->dirVec.x * cos(rotSpeed) - env->dirVec.y * sin(rotSpeed);
		env->dirVec.y = oldDirX * sin(rotSpeed) + env->dirVec.y * cos(rotSpeed);
		double oldPlaneX = env->planeVec.x;
		env->planeVec.x = env->planeVec.x * cos(rotSpeed) - env->planeVec.y * sin(rotSpeed);
		env->planeVec.y = oldPlaneX * sin(rotSpeed) + env->planeVec.y * cos(rotSpeed);
		refresh(env);
	}
	else if (!ft_mlx_move(keycode, env))
		return (0);
	return (0);
}

void	init_textures(t_env *env)
{
	int img_width = 64, img_height = 64;
	int bpp, n_bytes, endian;
	void *img = mlx_xpm_file_to_image(env->mlx, "092.xpm", &img_width, &img_height);
	if (!img)
		ft_exit(5, "error loading textures.\n");
	env->textures.north = mlx_get_data_addr(img, &bpp, &n_bytes, &endian);
	void *south = mlx_xpm_file_to_image(env->mlx, "092.xpm", &img_width, &img_height);
	if (!img)
		ft_exit(5, "error loading textures.\n");
	env->textures.south = mlx_get_data_addr(south, &bpp, &n_bytes, &endian);
	void *west = mlx_xpm_file_to_image(env->mlx, "091.xpm", &img_width, &img_height);
	if (!west)
		ft_exit(5, "error loading textures.\n");
	env->textures.west = mlx_get_data_addr(west, &bpp, &n_bytes, &endian);
	void *east = mlx_xpm_file_to_image(env->mlx, "091.xpm", &img_width, &img_height);
	if (!east)
		ft_exit(5, "error loading textures.\n");
	env->textures.east = mlx_get_data_addr(east, &bpp, &n_bytes, &endian);
	printf("textures loaded: %d, %d\n", img_width, img_height);
}

void init_mlx(t_env *env)
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
	env->img.addr = mlx_get_data_addr(env->img.img, &env->img.bpp,
									  &env->img.n_bytes, &env->img.endian);
	if (!env->img.addr)
		ft_exit(5, "error initializing image data.\n");
	init_textures(env);
}
