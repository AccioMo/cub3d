/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzeggaf <mzeggaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 21:54:05 by mzeggaf           #+#    #+#             */
/*   Updated: 2024/12/02 19:00:15 by mzeggaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	prepare_ray_textures(t_ray *ray)
{
	int	line_height;

	ray->tex.x = (int)(ray->wall_x * (double)TEXTURE_WIDTH);
	if ((ray->face == EAST || ray->face == WEST || ray->face == \
	HOR_DOOR_CLOSED || ray->face == VER_DOOR_CLOSED) && ray->dir.x > 0)
		ray->tex.x = TEXTURE_WIDTH - ray->tex.x - 1;
	if ((ray->face == NORTH || ray->face == SOUTH || ray->face == \
	HOR_DOOR_OPEN || ray->face == VER_DOOR_OPEN) && ray->dir.y < 0)
		ray->tex.x = TEXTURE_WIDTH - ray->tex.x - 1;
	line_height = ((int)(W_HEIGHT / ray->perp_wall_dist));
	ray->step = 1.0 * TEXTURE_HEIGHT / line_height;
	ray->draw_start = -line_height / 2 + W_HEIGHT / 2;
	if (ray->draw_start < 0)
		ray->draw_start = 0;
	ray->draw_end = line_height / 2 + W_HEIGHT / 2;
	if (ray->draw_end >= W_HEIGHT)
		ray->draw_end = W_HEIGHT - 1;
	ray->tex.y = (ray->draw_start - W_HEIGHT / 2 + line_height / 2) * ray->step;
}

static unsigned int	ft_draw_door_textures(t_ray *ray, t_env *env)
{
	unsigned int	color;

	if (ray->face == HOR_DOOR_CLOSED || ray->face == VER_DOOR_CLOSED)
		color = ((unsigned int *)env->textures.closed_door)[(int)((int) \
		ray->tex.y * TEXTURE_WIDTH + ray->tex.x)];
	else if (ray->face == HOR_DOOR_OPEN || ray->face == VER_DOOR_OPEN)
		color = ((unsigned int *)env->textures.open_door)[(int)((int) \
		ray->tex.y * TEXTURE_WIDTH + ray->tex.x)];
	else
		color = 0;
	return (color);
}

void	draw_ray_textures(t_ray *ray, int win_x, t_env *env)
{
	unsigned int	color;
	int				win_y;

	win_y = ray->draw_start;
	while (win_y < ray->draw_end)
	{
		if (ray->face == NORTH)
			color = ((unsigned int *)env->textures.north)[(int)((int) \
			ray->tex.y * TEXTURE_WIDTH + ray->tex.x)] >> 1 & 0x7F7F7F;
		else if (ray->face == SOUTH)
			color = ((unsigned int *)env->textures.south)[(int)((int) \
			ray->tex.y * TEXTURE_WIDTH + ray->tex.x)] >> 1 & 0x7F7F7F;
		else if (ray->face == EAST)
			color = ((unsigned int *)env->textures.east)[(int)((int) \
			ray->tex.y * TEXTURE_WIDTH + ray->tex.x)];
		else if (ray->face == WEST)
			color = ((unsigned int *)env->textures.west)[(int)((int) \
			ray->tex.y * TEXTURE_WIDTH + ray->tex.x)];
		else
			color = ft_draw_door_textures(ray, env);
		ft_put_pixel(win_x, win_y, color, &env->img);
		ray->tex.y += ray->step;
		win_y++;
	}
}
