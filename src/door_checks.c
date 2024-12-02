/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_checks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzeggaf <mzeggaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 22:54:06 by mzeggaf           #+#    #+#             */
/*   Updated: 2024/12/02 23:49:25 by mzeggaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	horizontal_closed_door_hit(t_env *env, t_ray *ray)
{
	if (env->map.blocks[(int)ray->hit.y][(int)ray->hit.x] == 'H' \
		&& ray->face == NORTH)
	{
		ray->face = HOR_DOOR_CLOSED;
		if (env->player.target == NULL)
			env->player.target = &env->map.blocks[(int)ray->hit.y] \
			[(int)ray->hit.x];
		return (1);
	}
	if ((int)ray->hit.y - 1 >= 0 && env->map.blocks[(int)ray->hit.y - 1] \
	[(int)ray->hit.x] == 'H' && ray->face == SOUTH)
	{
		ray->face = HOR_DOOR_CLOSED;
		if (env->player.target == NULL)
			env->player.target = &env->map.blocks[(int)ray->hit.y - 1] \
			[(int)ray->hit.x];
		return (1);
	}
	return (0);
}

static int	horizontal_open_door_hit(t_env *env, t_ray *ray)
{
	if ((int)ray->hit.x + 1 < env->map.width && env->map.blocks \
	[(int)ray->hit.y][(int)ray->hit.x + 1] == 'h' && ray->face == WEST)
	{
		ray->face = HOR_DOOR_OPEN;
		if (env->player.target == NULL)
			env->player.target = &env->map.blocks[(int)ray->hit.y] \
			[(int)ray->hit.x + 1];
		return (1);
	}
	if ((int)ray->hit.x - 1 > 0 && env->map.blocks[(int)ray->hit.y] \
	[(int)ray->hit.x - 1] == 'h' && ray->face == EAST)
	{
		ray->face = HOR_DOOR_OPEN;
		if (env->player.target == NULL)
			env->player.target = &env->map.blocks[(int)ray->hit.y] \
			[(int)ray->hit.x - 1];
		return (1);
	}
	return (0);
}

static int	vertical_closed_door_hit(t_env *env, t_ray *ray)
{
	if (env->map.blocks[(int)ray->hit.y][(int)ray->hit.x] == 'V' \
	&& ray->face == EAST)
	{
		ray->face = VER_DOOR_CLOSED;
		if (env->player.target == NULL)
			env->player.target = &env->map.blocks[(int)ray->hit.y] \
			[(int)ray->hit.x];
		return (1);
	}
	if ((int)ray->hit.x - 1 >= 0 && env->map.blocks[(int)ray->hit.y] \
	[(int)ray->hit.x + 1] == 'V' && ray->face == WEST)
	{
		ray->face = VER_DOOR_CLOSED;
		if (env->player.target == NULL)
			env->player.target = &env->map.blocks[(int)ray->hit.y] \
			[(int)ray->hit.x + 1];
		return (1);
	}
	return (0);
}

static int	vertical_open_door_hit(t_env *env, t_ray *ray)
{
	if ((int)ray->hit.y + 1 < env->map.height && env->map.blocks \
	[(int)ray->hit.y + 1][(int)ray->hit.x] == 'v' && ray->face == NORTH)
	{
		ray->face = VER_DOOR_OPEN;
		if (env->player.target == NULL)
			env->player.target = &env->map.blocks[(int)ray->hit.y + 1] \
			[(int)ray->hit.x];
		return (1);
	}
	if ((int)ray->hit.y - 1 > 0 && env->map.blocks[(int)ray->hit.y - 1] \
	[(int)ray->hit.x] == 'v' && ray->face == SOUTH)
	{
		ray->face = VER_DOOR_OPEN;
		if (env->player.target == NULL)
			env->player.target = &env->map.blocks[(int)ray->hit.y - 1] \
			[(int)ray->hit.x];
		return (1);
	}
	return (0);
}

int	door_hit(t_env *env, t_ray *ray)
{
	if (horizontal_closed_door_hit(env, ray) || \
	horizontal_open_door_hit(env, ray))
		return (1);
	if (vertical_closed_door_hit(env, ray) || \
	vertical_open_door_hit(env, ray))
		return (1);
	else
		return (0);
}
