/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algorithm.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzeggaf <mzeggaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 22:08:05 by mzeggaf           #+#    #+#             */
/*   Updated: 2024/12/02 22:33:03 by mzeggaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	hit_a_wall(t_ray *ray, t_env *env)
{
	if ((int)ray->hit.y >= env->map.height)
		return (1);
	if ((int)ray->hit.x >= env->map.width)
		return (1);
	if ((int)ray->hit.x < 0 || (int)ray->hit.y < 0)
		return (1);
	if (door_hit(env, ray))
		return (1);
	if (env->map.blocks[(int)ray->hit.y][(int)ray->hit.x] == '1')
		return (1);
	return (0);
}

static void	horizontal_step(t_ray *ray, int stepx)
{
	ray->side.x += ray->delta.x;
	ray->hit.x += stepx;
	if (stepx > 0)
		ray->face = EAST;
	else
		ray->face = WEST;
}

static void	vertical_step(t_ray *ray, int stepy)
{
	ray->side.y += ray->delta.y;
	ray->hit.y += stepy;
	if (stepy > 0)
		ray->face = SOUTH;
	else
		ray->face = NORTH;
}

void	dda(t_ray *ray, t_env *env)
{
	int		stepx;
	int		stepy;

	ray->hit.x = env->player.x;
	ray->hit.y = env->player.y;
	stepx = -(ray->dir.x < 0) * 2 + 1;
	stepy = -(ray->dir.y < 0) * 2 + 1;
	while (0b1)
	{
		if (ray->side.x < ray->side.y)
			horizontal_step(ray, stepx);
		else
			vertical_step(ray, stepy);
		if (hit_a_wall(ray, env))
			return ;
	}
}

void	flood_fill(t_env *env, char **lines, int x, int y)
{
	int	len;

	len = ft_strlen(lines[y]);
	if (x >= len || x < 0)
		ft_exit(0, "map is not surrounded\n");
	if (y >= env->map.height || y < 0)
		ft_exit(0, "map is not surrounded\n");
	if (lines[y][x] == ' ')
		ft_exit(0, "map is not surrounded\n");
	if (lines[y][x] == 'x' || lines[y][x] == '1')
		return ;
	lines[y][x] = 'x';
	flood_fill(env, lines, x + 1, y);
	flood_fill(env, lines, x - 1, y);
	flood_fill(env, lines, x, y + 1);
	flood_fill(env, lines, x, y - 1);
}
