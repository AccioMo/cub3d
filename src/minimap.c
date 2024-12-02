/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzeggaf <mzeggaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 19:49:17 by mzeggaf           #+#    #+#             */
/*   Updated: 2024/12/02 23:06:27 by mzeggaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	convert_to_unit_vector(t_point *u)
{
	u->x = u->x / sqrt(u->x * u->x + u->y * u->y);
	u->y = u->y / sqrt(u->x * u->x + u->y * u->y);
}

int	ft_get_pixel(int x, int y, t_image *img)
{
	char	*dst;

	x *= (img->bpp / 8);
	y *= img->n_bytes;
	dst = img->addr + x + y;
	return (*(unsigned int *)dst);
}

void	ft_draw_player(t_image *img)
{
	t_block	block;

	block.x = MINIMAP_WIDTH / 2 - 2;
	block.y = MINIMAP_WIDTH / 2 - 2;
	block.width = 5;
	block.height = 5;
	ft_paint_block(&block, 0xff0000, img);
}

void	mapimg_to_minimap(t_env *env, t_image *mapimg, t_image *minimap)
{
	int	left_pad;
	int	top_pad;
	int	x;
	int	y;

	left_pad = env->player.x * BLOCK_SIZE - MINIMAP_WIDTH / 2;
	top_pad = env->player.y * BLOCK_SIZE - MINIMAP_HEIGHT / 2;
	x = 0;
	while (x < MINIMAP_WIDTH)
	{
		y = 0;
		while (y < MINIMAP_HEIGHT)
		{
			if ((x + left_pad >= 0 && x + left_pad < env->map.width * \
			BLOCK_SIZE) && (y + top_pad >= 0 && y + top_pad < env->map.height \
			* BLOCK_SIZE))
				ft_put_pixel(x, y, ft_get_pixel(x + left_pad, y + top_pad, \
				mapimg), minimap);
			else
				ft_put_pixel(x, y, 0x0, minimap);
			y++;
		}
		x++;
	}
	ft_draw_player(minimap);
}

void	map_to_img(t_env *env, t_image *img)
{
	t_block	block;
	int		color;
	int		i;
	int		j;

	i = 0;
	block.width = BLOCK_SIZE;
	block.height = BLOCK_SIZE;
	while (i < env->map.height)
	{
		j = 0;
		while (j < env->map.width)
		{
			block.x = j * BLOCK_SIZE;
			block.y = i * BLOCK_SIZE;
			if (env->map.blocks[i][j] == '0')
				color = 0xFFFFFF;
			else
				color = 0x0;
			ft_paint_block(&block, color, img);
			j++;
		}
		i++;
	}
}
