/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzeggaf <mzeggaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 22:39:13 by mzeggaf           #+#    #+#             */
/*   Updated: 2024/12/02 23:50:20 by mzeggaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// TODO: remove this: DONE (removed that)

static int	door_is_horizontal(t_env *env, int x, int y)
{
	if (x - 1 < 0 || x + 1 >= env->map.width)
		return (0);
	if (env->map.blocks[y][x - 1] == '1' && env->map.blocks[y][x + 1] == '1')
		return (1);
	else
		return (0);
}

static int	door_is_vertical(t_env *env, int x, int y)
{
	if (y - 1 < 0 || y + 1 >= env->map.height)
		return (0);
	if (env->map.blocks[y - 1][x] == '1' && env->map.blocks[y + 1][x] == '1')
		return (1);
	else
		return (0);
}

void	catigorize_doors(t_env *env)
{
	int	i;
	int	j;

	i = -1;
	while (++i < env->map.height)
	{
		j = -1;
		while (++j < env->map.width)
		{
			if (env->map.blocks[i][j] == 'D')
			{
				if (door_is_vertical(env, j, i))
					env->map.blocks[i][j] = 'V';
				else if (door_is_horizontal(env, j, i))
					env->map.blocks[i][j] = 'H';
			}
			else if (env->map.blocks[i][j] == 'd')
			{
				if (door_is_vertical(env, j, i))
					env->map.blocks[i][j] = 'v';
				else if (door_is_horizontal(env, j, i))
					env->map.blocks[i][j] = 'h';
			}
		}
	}
}

void	toggle_door(t_env *env)
{
	if (*env->player.target == 'V' || *env->player.target == 'H')
		*env->player.target = ft_tolower(*env->player.target);
	else if (*env->player.target == 'v' || *env->player.target == 'h')
		*env->player.target = ft_toupper(*env->player.target);
}
