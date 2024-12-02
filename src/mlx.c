/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzeggaf <mzeggaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 15:44:10 by mzeggaf           #+#    #+#             */
/*   Updated: 2024/12/02 23:50:42 by mzeggaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	ft_keydown_event(int keycode, void *ptr)
{
	t_env	*env;

	env = (t_env *)ptr;
	if (keycode == ESC)
		ft_end(env);
	else if (keycode == KEY_LEFT)
		env->player.rotate = R_LEFT;
	else if (keycode == KEY_RIGHT)
		env->player.rotate = R_RIGHT;
	else if (keycode == KEY_D)
		env->player.move = M_RIGHT;
	else if (keycode == KEY_A)
		env->player.move = M_LEFT;
	else if (keycode == KEY_W)
		env->player.move = M_FORWARD;
	else if (keycode == KEY_S)
		env->player.move = M_BACKWARD;
	else if (keycode == KEY_R_SHIFT || keycode == KEY_L_SHIFT)
		mlx_mouse_show();
	return (0);
}

int	ft_keyup_event(int keycode, void *ptr)
{
	t_env	*env;

	env = (t_env *)ptr;
	(void)keycode;
	if (keycode == KEY_LEFT || keycode == KEY_RIGHT)
		env->player.rotate = R_NONE;
	else if (keycode == KEY_D || keycode == KEY_A)
		env->player.move = M_NONE;
	else if (keycode == KEY_W || keycode == KEY_S)
		env->player.move = M_NONE;
	else if (keycode == KEY_R_SHIFT || keycode == KEY_L_SHIFT)
		mlx_mouse_hide();
	return (0);
}

void	init_map_and_minimap_imgs(t_env *env)
{
	env->mapimg.img = mlx_new_image(env->mlx, env->map.width * BLOCK_SIZE,
			env->map.height * BLOCK_SIZE);
	if (env->mapimg.img == NULL)
		ft_exit(0, "failed to allocate minimap\n");
	env->mapimg.addr = mlx_get_data_addr(env->mapimg.img, &env->mapimg.bpp,
			&env->mapimg.n_bytes, &env->mapimg.endian);
	if (!env->mapimg.addr)
		ft_exit(5, "error initializing image data.\n");
	map_to_img(env, &env->mapimg);
	env->minimap.img = mlx_new_image(env->mlx, MINIMAP_WIDTH, MINIMAP_HEIGHT);
	if (env->minimap.img == NULL)
		ft_exit(0, "failed to allocate minimap\n");
	env->minimap.addr = mlx_get_data_addr(env->minimap.img, &env->minimap.bpp,
			&env->minimap.n_bytes, &env->minimap.endian);
	if (!env->minimap.addr)
		ft_exit(5, "error initializing image data.\n");
}

void	init_mlx(t_env *env)
{
	env->mlx = mlx_init();
	if (!env->mlx)
		ft_exit(5, "init error.\n");
	env->win = mlx_new_window(env->mlx, W_WIDTH, W_HEIGHT, "cub3d");
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
	init_map_and_minimap_imgs(env);
}
