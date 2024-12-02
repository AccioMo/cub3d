/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzeggaf <mzeggaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 19:01:28 by mzeggaf           #+#    #+#             */
/*   Updated: 2024/12/02 19:32:57 by mzeggaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	ft_rotate_left(double rotation_speed, t_env *env)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = env->dir.x;
	env->dir.x = env->dir.x * cos(-rotation_speed) - env->dir.y
		* sin(-rotation_speed);
	env->dir.y = old_dir_x * sin(-rotation_speed) + env->dir.y
		* cos(-rotation_speed);
	old_plane_x = env->plane.x;
	env->plane.x = env->plane.x * cos(-rotation_speed) - env->plane.y
		* sin(-rotation_speed);
	env->plane.y = old_plane_x * sin(-rotation_speed) + env->plane.y
		* cos(-rotation_speed);
}

void	ft_rotate_right(double rotation_speed, t_env *env)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = env->dir.x;
	env->dir.x = env->dir.x * cos(rotation_speed) - env->dir.y
		* sin(rotation_speed);
	env->dir.y = old_dir_x * sin(rotation_speed) + env->dir.y
		* cos(rotation_speed);
	old_plane_x = env->plane.x;
	env->plane.x = env->plane.x * cos(rotation_speed) - env->plane.y
		* sin(rotation_speed);
	env->plane.y = old_plane_x * sin(rotation_speed) + env->plane.y
		* cos(rotation_speed);
}

int	ft_mouse_hook(int code, int x, int y, void *param)
{
	t_env	*env;

	env = (t_env *)param;
	(void)x;
	(void)y;
	if (code == 2)
	{
		toggle_door(env);
	}
	return (0);
}

static void	handle_mouse_move(int x, void *param)
{
	t_env	*env;

	env = (t_env *)param;
	if (env->mouse.x < x)
	{
		env->mouse.rotate = R_LEFT;
		env->mouse.acceleration += 0.1 * (1 - env->mouse.acceleration);
		if (env->mouse.acceleration > 1)
			env->mouse.acceleration = 1;
	}
	else if (env->mouse.x > x)
	{
		env->mouse.rotate = R_RIGHT;
		env->mouse.acceleration += 0.1 * (1 - env->mouse.acceleration);
		if (env->mouse.acceleration > 1)
			env->mouse.acceleration = 1;
	}
}

int	ft_mouse_move(int x, int y, void *param)
{
	t_env	*env;

	env = (t_env *)param;
	handle_mouse_move(x, param);
	env->mouse.x = x;
	env->mouse.y = y;
	if (x >= W_WIDTH)
	{
		mlx_mouse_move(env->win, 0, W_HEIGHT / 2);
		env->mouse.x = 0;
	}
	else if (x < 0)
	{
		mlx_mouse_move(env->win, W_WIDTH - 1, W_HEIGHT / 2);
		env->mouse.x = W_WIDTH - 1;
	}
	return (1);
}
