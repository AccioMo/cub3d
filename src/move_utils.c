/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzeggaf <mzeggaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 19:44:20 by mzeggaf           #+#    #+#             */
/*   Updated: 2024/12/02 19:44:33 by mzeggaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	is_wall(char c)
{
	if (c == '0' || c == 'v' || c == 'h')
		return (0);
	else
		return (1);
}

char	prev_block(t_env *env, float *coord)
{
	char	block;

	*coord -= 1;
	block = env->map.blocks[(int)env->player.y][(int)env->player.x];
	*coord += 1;
	return (block);
}
