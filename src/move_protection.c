/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_protection.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzeggaf <mzeggaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 01:44:26 by ayait-el          #+#    #+#             */
/*   Updated: 2024/12/02 19:44:03 by mzeggaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static char	next_block(t_env *env, float *coord)
{
	char	block;

	*coord += 1;
	block = env->map.blocks[(int)env->player.y][(int)env->player.x];
	*coord -= 1;
	return (block);
}

static int	check_prev_corner(t_env *env, float *coord)
{
	if (coord == &env->player.x)
	{
		if (is_wall(env->map.blocks[(int)env->player.y + 1][(int)env->player.x
				- 1]) && env->player.y - (int)env->player.y > 0.9)
			return (1);
		else if (is_wall(env->map.blocks[(int)env->player.y
					- 1][(int)env->player.x - 1]) && env->player.y
					- (int)env->player.y < 0.1)
			return (1);
	}
	else
	{
		if (is_wall(env->map.blocks[(int)env->player.y - 1][(int)env->player.x
				+ 1]) && env->player.x - (int)env->player.x > 0.9)
			return (1);
		else if (is_wall(env->map.blocks[(int)env->player.y
					- 1][(int)env->player.x - 1]) && env->player.x
					- (int)env->player.x < 0.1)
			return (1);
	}
	return (0);
}

static int	check_next_corner(t_env *env, float *coord)
{
	if (coord == &env->player.x)
	{
		if (is_wall(env->map.blocks[(int)env->player.y + 1][(int)env->player.x
				+ 1]) && env->player.y - (int)env->player.y > 0.9)
			return (1);
		else if (is_wall(env->map.blocks[(int)env->player.y
					- 1][(int)env->player.x + 1]) && env->player.y
					- (int)env->player.y < 0.1)
			return (1);
	}
	else
	{
		if (is_wall(env->map.blocks[(int)env->player.y + 1][(int)env->player.x
				+ 1]) && env->player.x - (int)env->player.x > 0.9)
			return (1);
		else if (is_wall(env->map.blocks[(int)env->player.y
					+ 1][(int)env->player.x - 1]) && env->player.x
					- (int)env->player.x < 0.1)
			return (1);
	}
	return (0);
}

void	coord_increase_protection(t_env *env, float *coord, double dirVec_coord)
{
	if (!is_wall(env->map.blocks[(int)env->player.y][(int)env->player.x]))
	{
		if (dirVec_coord > 0)
		{
			if ((check_next_corner(env, coord) || is_wall(next_block(env,
							coord))) && *coord - (int)*coord > 0.9)
				*coord = ((int)*coord) + 0.9;
		}
		else
		{
			if ((check_prev_corner(env, coord) || is_wall(prev_block(env,
							coord))) && *coord - (int)*coord < 0.1)
				*coord = ((int)*coord) + 0.1;
		}
	}
	else
	{
		if (dirVec_coord > 0)
			*coord = (int)*coord - 1 + 0.9;
		else
			*coord = (int)*coord + 1 + 0.1;
	}
}

void	coord_decrease_protection(t_env *env, float *coord, double dirVec_coord)
{
	if (!is_wall(env->map.blocks[(int)env->player.y][(int)env->player.x]))
	{
		if (dirVec_coord < 0)
		{
			if ((check_next_corner(env, coord) || is_wall(next_block(env,
							coord))) && *coord - (int)*coord > 0.9)
				*coord = ((int)*coord) + 0.9;
		}
		else
		{
			if ((check_prev_corner(env, coord) || is_wall(prev_block(env,
							coord))) && *coord - (int)*coord < 0.1)
				*coord = ((int)*coord) + 0.1;
		}
	}
	else
	{
		if (dirVec_coord < 0)
			*coord = (int)*coord - 1 + 0.9;
		else
			*coord = (int)*coord + 1 + 0.1;
	}
}
