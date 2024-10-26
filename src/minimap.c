/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzeggaf <mzeggaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 15:53:36 by mzeggaf           #+#    #+#             */
/*   Updated: 2024/10/23 16:20:23 by mzeggaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// static t_point get_hit_point(t_player *player, t_point hit, t_vector rayDir)
// {
// 	t_point result;
// 	int x;
// 	int y;

// 	x = fabs(hit.x * BLOCK_SIZE - (int)(player->x + MP_OFFSET + MP_BORDER)) < fabs((hit.x + 1) * BLOCK_SIZE - (int)(player->x + MP_OFFSET + MP_BORDER)) ? hit.x * BLOCK_SIZE : (hit.x + 1) * BLOCK_SIZE;
// 	y = fabs(hit.y * BLOCK_SIZE - (int)(player->y + MP_OFFSET + MP_BORDER)) < fabs((hit.y + 1) * BLOCK_SIZE - (int)(player->y + MP_OFFSET + MP_BORDER)) ? hit.y * BLOCK_SIZE : (hit.y + 1) * BLOCK_SIZE;

// 	double m = rayDir.y / rayDir.x;
// 	double c = (int)(player->y + MP_OFFSET + MP_BORDER) - m * (int)(player->x + MP_OFFSET + MP_BORDER);
// 	if (hit.color == 0)
// 	{
// 		result.x = x;
// 		result.y = m * x + c;
// 	}
// 	else
// 	{
// 		result.y = y;
// 		result.x = (y - c) / m;
// 	}
// 	return (result);
// }

void draw_map(t_env *env)
{
	t_block	block;
	t_map *map = &env->map;
	t_point map_pos;
	t_point map_offset;
	map_pos.x = env->player.x / BLOCK_SIZE;
	map_pos.y = env->player.y / BLOCK_SIZE;
	map_offset.x = env->player.x % BLOCK_SIZE;
	map_offset.y = env->player.y % BLOCK_SIZE;
	int start_h;
	int start_w;
	int end_h;
	int end_w;

	env->map.x = 0;
	env->map.y = 0;
	if ((map_pos.y - MINIMAP_BLOCKS / 2) >= 0)
		start_h = map_pos.y - MINIMAP_BLOCKS / 2;
	else
	{
		start_h = 0;
		env->map.y = (map_pos.y - MINIMAP_BLOCKS / 2) * BLOCK_SIZE + map_offset.y;
		map_offset.y = 0;
	}
	if ((map_pos.x - MINIMAP_BLOCKS / 2) >= 0)
		start_w = map_pos.x - MINIMAP_BLOCKS / 2;
	else
	{
		start_w = 0;
		env->map.x = (map_pos.x - MINIMAP_BLOCKS / 2) * BLOCK_SIZE + map_offset.x;
		map_offset.x = 0;
	}

	if (start_h + MINIMAP_BLOCKS + 1 <= map->height)
		end_h = start_h + MINIMAP_BLOCKS + 1;
	else
	{
		env->map.y = (start_h + MINIMAP_BLOCKS - map->height) * BLOCK_SIZE + map_offset.y;
		start_h = map->height - MINIMAP_BLOCKS - 1;
		end_h = map->height;
		map_offset.y = BLOCK_SIZE;
	}
	if (start_w + MINIMAP_BLOCKS + 1 <= map->width)
		end_w = start_w + MINIMAP_BLOCKS + 1;
	else
	{
		env->map.x = (start_w + MINIMAP_BLOCKS - map->width) * BLOCK_SIZE + map_offset.x;
		start_w = map->width - MINIMAP_BLOCKS - 1;
		end_w = map->width;
		map_offset.x = BLOCK_SIZE;
	}

	int j;
	int i = 0;
	int h = BLOCK_SIZE;
	int w = BLOCK_SIZE;
	block = (t_block){MP_OFFSET, MP_OFFSET, MINIMAP_BLOCKS * BLOCK_SIZE + MP_BORDER * 2, MINIMAP_BLOCKS * BLOCK_SIZE + MP_BORDER * 2};
	ft_paint_block(&block, 0xFF0000, env);
	while (start_h + i < end_h)
	{
		j = 0;
		while (start_w + j < end_w)
		{
			h = BLOCK_SIZE;
			w = BLOCK_SIZE;
			int x = j * BLOCK_SIZE - map_offset.x;
			int y = i * BLOCK_SIZE - map_offset.y;
			if (i == 0)
			{
				h = BLOCK_SIZE - map_offset.y;
				y = 0;
			}
			else if (i == end_h - start_h - 1)
				h = map_offset.y;
			if (j == 0)
			{
				w = BLOCK_SIZE - map_offset.x;
				x = 0;
			}
			else if (j == end_w - start_w - 1)
				w = map_offset.x;
			block = (t_block){x + MP_OFFSET + MP_BORDER, y + MP_OFFSET + MP_BORDER, w, h};
			ft_paint_block(&block, map->blocks[start_h + i][start_w + j] * 0xFFFFFF, env);
			j++;
		}
		i++;
	}
}

void draw_player(t_env *env)
{
	double i = 0;
	double angle;
	double x1;
	double x2;
	double y1;
	double r = BLOCK_SIZE / 10;
	t_point player = {(MINIMAP_BLOCKS / 2) * BLOCK_SIZE, (MINIMAP_BLOCKS / 2) * BLOCK_SIZE, 0xFF0000};
	if (env->map.x || env->map.y)
	{
		player.x = player.x + env->map.x;
		player.y = player.y + env->map.y;
	}
	while (i < 360)
	{
		angle = i;
		x1 = r * cos(angle * M_PI / 180);
		y1 = r * sin(angle * M_PI / 180);
		x2 = round(player.x + x1);
		while (x2 != round(player.x))
		{
			ft_put_pixel(x2, round(player.y + y1), 0xFFFFFF, &env->img);
			if (x2 < round(player.x))
				x2 += 1;
			else
				x2 -= 1;
		}
		ft_put_pixel(x2, round(player.y + y1), 0xFFFFFF, &env->img);
		i += 0.1f;
	}
}

int draw_minimap(t_env *env)
{
	draw_map(env);
	// for (int i = 0; i < W_WIDTH; i++)
	// {
	// 	double windowX = 2 * i / (double)W_WIDTH - 1;
	// 	t_vector rayDir = {env->dirVec.x + env->planeVec.x * windowX, env->dirVec.y + env->planeVec.y * windowX};

	// 	double deltax = (rayDir.x == 0) ? BLOCK_SIZE : fabs(1 / rayDir.x) * (double)BLOCK_SIZE;
	// 	double deltay = (rayDir.y == 0) ? BLOCK_SIZE : fabs(1 / rayDir.y) * (double)BLOCK_SIZE;
	// 	double initstepy;
	// 	double initstepx;
	// 	if (rayDir.x < 0)
	// 		initstepx = deltax * (((env->player.x + MP_OFFSET + MP_BORDER) % BLOCK_SIZE) / (double)BLOCK_SIZE);
	// 	else
	// 		initstepx = deltax * ((BLOCK_SIZE - (env->player.x + MP_OFFSET + MP_BORDER) % BLOCK_SIZE) / (double)BLOCK_SIZE);
	// 	if (rayDir.y < 0)
	// 		initstepy = deltay * (((env->player.y + MP_OFFSET + MP_BORDER) % BLOCK_SIZE) / (double)BLOCK_SIZE);
	// 	else
	// 		initstepy = deltay * ((BLOCK_SIZE - (env->player.y + MP_OFFSET + MP_BORDER) % BLOCK_SIZE) / (double)BLOCK_SIZE);

	// 	t_point delta = {deltax, deltay, 0x008000};
	// 	t_point side = {initstepx, initstepy, 0x008000};
	// 	t_point wall = dda(env, &delta, rayDir, &side);
	// 	t_point x = get_hit_point(&env->player, wall, rayDir);
	// 	t_player player = {MINIMAP_SIZE / 2 + MP_OFFSET + MP_BORDER, MINIMAP_SIZE / 2 + MP_OFFSET + MP_BORDER, 0};
	// 	if (env->map.x || env->map.y)
	// 	{
	// 		player.x = player.x + env->map.x;
	// 		player.y = player.y + env->map.y;
	// 	}
	// 	x.x -= env->player.x - player.x;
	// 	x.y -= env->player.y - player.y;
	// 	ft_aa_draw(&(t_point){player.x, player.y, 0xFFFFFF}, &x, &env->img, 0x808080);
	// }
	draw_player(env);
	return (0);
}
