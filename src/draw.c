/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzeggaf <mzeggaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 18:02:01 by mzeggaf           #+#    #+#             */
/*   Updated: 2024/12/02 22:10:11 by mzeggaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	ft_put_pixel(int x, int y, int color, t_image *img)
{
	char	*dst;

	if ((0 <= x && x < W_WIDTH) && (0 <= y && y < W_HEIGHT))
	{
		x *= (img->bpp / 8);
		y *= img->n_bytes;
		dst = img->addr + x + y;
		*(unsigned int *)dst = color;
	}
}

void	ft_paint_block(t_block *block, int color, t_image *img)
{
	int	h;
	int	w;

	h = 0;
	while (h < block->height)
	{
		w = 0;
		while (w < block->width)
		{
			ft_put_pixel(w + block->x, h + block->y, color, img);
			w += 1;
		}
		h += 1;
	}
}

void	draw_crosshair(t_env *env)
{
	t_block	x;
	t_block	y;

	x.x = (W_WIDTH / 2) - (CURSOR_SIZE / 2 - 1);
	x.y = (W_HEIGHT / 2);
	x.width = CURSOR_SIZE;
	x.height = 2;
	y.x = (W_WIDTH / 2);
	y.y = (W_HEIGHT / 2) - (CURSOR_SIZE / 2 - 1);
	y.height = CURSOR_SIZE;
	y.width = 2;
	ft_paint_block(&x, 0xFFFFFF, &env->img);
	ft_paint_block(&y, 0xFFFFFF, &env->img);
}
