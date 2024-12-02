/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzeggaf <mzeggaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 21:53:18 by mzeggaf           #+#    #+#             */
/*   Updated: 2024/12/02 23:08:26 by mzeggaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	cast_ray(t_ray *ray, double ray_x, t_env *env)
{
	ray->dir.x = env->dir.x + env->plane.x * ray_x;
	ray->dir.y = env->dir.y + env->plane.y * ray_x;
	if (ray->dir.x == 0)
		ray->delta.x = 1e30;
	else
		ray->delta.x = fabs(1 / ray->dir.x);
	if (ray->dir.y == 0)
		ray->delta.y = 1e30;
	else
		ray->delta.y = fabs(1 / ray->dir.y);
	if (ray->dir.x < 0)
		ray->side.x = ray->delta.x * (env->player.x - (int)env->player.x);
	else
		ray->side.x = ray->delta.x * ((int)env->player.x + 1.0 - env->player.x);
	if (ray->dir.y < 0)
		ray->side.y = ray->delta.y * (env->player.y - (int)env->player.y);
	else
		ray->side.y = ray->delta.y * ((int)env->player.y + 1.0 - env->player.y);
}

static void	get_perp_dist(t_ray *ray, double *pdb, int win_x, t_env *env)
{
	if (ray->face == WEST || ray->face == EAST || \
	ray->face == HOR_DOOR_OPEN || ray->face == VER_DOOR_CLOSED)
	{
		ray->perp_wall_dist = ray->side.x - fabs(ray->delta.x);
		ray->wall_x = env->player.y + ray->perp_wall_dist * ray->dir.y;
	}
	else
	{
		ray->perp_wall_dist = ray->side.y - fabs(ray->delta.y);
		ray->wall_x = env->player.x + ray->perp_wall_dist * ray->dir.x;
	}
	pdb[win_x] = ray->perp_wall_dist;
	ray->wall_x -= floor(ray->wall_x);
}

static int	render_screen(double perp_dist_buffer[W_WIDTH], t_env *env)
{
	double	ray_x;
	t_ray	ray;
	int		win_x;

	win_x = 0;
	ft_paint_block(&(t_block){0, 0, W_WIDTH, W_HEIGHT / 2}, \
		env->floor_color, &env->img);
	ft_paint_block(&(t_block){0, W_HEIGHT / 2, W_WIDTH, W_HEIGHT / 2}, \
		env->ceiling_color, &env->img);
	while (win_x < W_WIDTH)
	{
		if (win_x == W_WIDTH / 2)
			env->player.target = NULL;
		ray_x = 2 * win_x / (double)(W_WIDTH) - 1;
		cast_ray(&ray, ray_x, env);
		dda(&ray, env);
		get_perp_dist(&ray, perp_dist_buffer, win_x, env);
		prepare_ray_textures(&ray);
		draw_ray_textures(&ray, win_x, env);
		if (win_x == W_WIDTH / 2 && env->player.target == NULL)
			env->player.target = \
				&env->map.blocks[(int)ray.hit.y][(int)ray.hit.x];
		win_x++;
	}
	return (0);
}

static void	player_controls(t_env *env)
{
	if (env->player.move != M_NONE)
		ft_mlx_move(env);
	if (env->player.rotate == R_LEFT)
		ft_rotate_left(ROTATION_SPEED * 0.3, env);
	else if (env->player.rotate == R_RIGHT)
		ft_rotate_right(ROTATION_SPEED * 0.3, env);
	if (env->mouse.rotate == R_LEFT)
		ft_rotate_right(ROTATION_SPEED * env->mouse.acceleration, env);
	else if (env->mouse.rotate == R_RIGHT)
		ft_rotate_left(ROTATION_SPEED * env->mouse.acceleration, env);
	else
	{
		env->mouse.acceleration *= 0.8;
		if (env->mouse.acceleration < 0.01)
			env->mouse.acceleration = 0;
	}
	env->mouse.rotate = R_NONE;
}

int	refresh(t_env *env)
{
	static int		f = 0;
	static double	next_sprite_frame;
	static double	last_frame;
	double			perp_dist_buffer[W_WIDTH];

	last_frame = ft_get_time();
	mapimg_to_minimap(env, &env->mapimg, &env->minimap);
	map_to_img(env, &env->mapimg);
	render_screen(perp_dist_buffer, env);
	render_sprite(perp_dist_buffer, f, env);
	if (last_frame + 16 > ft_get_time())
		usleep((last_frame + 16) - ft_get_time());
	player_controls(env);
	draw_crosshair(env);
	mlx_put_image_to_window(env->mlx, env->win, env->img.img, 0, 0);
	mlx_put_image_to_window(env->mlx, env->win, env->minimap.img, 0, 0);
	if (next_sprite_frame == 0)
		next_sprite_frame = ft_get_time() + 300;
	if (next_sprite_frame <= ft_get_time())
	{
		f = (f + (f < 2)) * (f < 2);
		next_sprite_frame = ft_get_time() + 300;
	}
	return (0);
}
