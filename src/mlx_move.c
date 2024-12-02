/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_move.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzeggaf <mzeggaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 01:30:34 by ayait-el          #+#    #+#             */
/*   Updated: 2024/12/02 19:32:48 by mzeggaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	move_right(t_env *env)
{
	t_player	*player;

	player = &env->player;
	player->x -= (env->dir.y * MOVE_SPEED);
	coord_decrease_protection(env, &player->x, env->dir.y);
	player->y += (env->dir.x * MOVE_SPEED);
	coord_increase_protection(env, &player->y, env->dir.x);
}

static void	move_left(t_env *env)
{
	t_player	*player;

	player = &env->player;
	player->x += (env->dir.y * MOVE_SPEED);
	coord_increase_protection(env, &player->x, env->dir.y);
	player->y -= (env->dir.x * MOVE_SPEED);
	coord_decrease_protection(env, &player->y, env->dir.x);
}

static void	move_forward(t_env *env)
{
	t_player	*player;

	player = &env->player;
	player->x += (env->dir.x * MOVE_SPEED);
	coord_increase_protection(env, &player->x, env->dir.x);
	player->y += (env->dir.y * MOVE_SPEED);
	coord_increase_protection(env, &player->y, env->dir.y);
}

static void	move_backward(t_env *env)
{
	t_player	*player;

	player = &env->player;
	player->x -= (env->dir.x * MOVE_SPEED);
	coord_decrease_protection(env, &player->x, env->dir.x);
	player->y -= (env->dir.y * MOVE_SPEED);
	coord_decrease_protection(env, &player->y, env->dir.y);
}

int	ft_mlx_move(t_env *env)
{
	t_player	*player;

	player = &env->player;
	if (player->move == M_RIGHT)
	{
		move_right(env);
		return (0);
	}
	else if (player->move == M_LEFT)
	{
		move_left(env);
		return (0);
	}
	else if (player->move == M_FORWARD)
	{
		move_forward(env);
		return (0);
	}
	else if (player->move == M_BACKWARD)
	{
		move_backward(env);
		return (0);
	}
	return (1);
}
