/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzeggaf <mzeggaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 18:02:01 by mzeggaf           #+#    #+#             */
/*   Updated: 2024/10/23 16:21:39 by mzeggaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	ft_swap(float *a, float *b)
{
	float	c;

	c = *a;
	*a = *b;
	*b = c;
}

static float	ft_get_step(t_line *vector)
{
	float	dx;
	float	dy;

	vector->rev = 0;
	if (vector->vert)
	{
		ft_swap(&vector->a.x, &vector->a.y);
		ft_swap(&vector->b.x, &vector->b.y);
	}
	if (vector->a.x > vector->b.x)
	{
		ft_swap(&vector->a.x, &vector->b.x);
		ft_swap(&vector->a.y, &vector->b.y);
		vector->rev = 1;
	}
	dx = vector->b.x - vector->a.x;
	dy = vector->b.y - vector->a.y;
	if (dx == 0)
		return (1);
	return (dy / dx);
}

static void	ft_draw_line(t_line *v, t_image *img, int color)
{
	//int	c;

	v->dx = v->b.x - v->a.x;
	if (v->vert)
	{
		while (v->a.x <= v->b.x && v->a.x < MINIMAP_SIZE)
		{
			ft_put_pixel(v->a.y, v->a.x, color, img);
			ft_put_pixel(v->a.y + 1, v->a.x, color, img);
			v->a.y += v->step;
			v->a.x++;
		}
	}
	else
	{
		while (v->a.x <= v->b.x && v->a.x < MINIMAP_SIZE)
		{
			ft_put_pixel(v->a.x, v->a.y, color, img);
			ft_put_pixel(v->a.x, v->a.y + 1, color, img);
			v->a.y += v->step;
			v->a.x++;
		}
	}
}

void ft_put_pixel(int x, int y, int color, t_image *img)
{
	char *dst;

	if ((0 <= x && x < W_WIDTH) && (0 <= y && y < W_HEIGHT))
	{
		x *= (img->bpp / 8);
		y *= img->n_bytes;
		dst = img->addr + x + y;
		*(unsigned int *)dst = color;
	}
}

void ft_paint_block(t_block *block, int color, t_env *env)
{
	int h;
	int w;

	h = 0;
	while (h < block->height)
	{
		w = 0;
		while (w < block->width)
		{
			// TODO: this adds the red lines grid, maybe remove later
			// if (w == 0 || h == 0)
			// 	ft_put_pixel(w + block->x, h + block->y, 0x808080, &env->img);
			// else
				ft_put_pixel(w + block->x, h + block->y, color, &env->img);
			w += 1;
		}
		h += 1;
	}
}

void	ft_aa_draw(t_point *a, t_point *b, t_image *img, int color)
{
	t_line	vct;

	vct.a = *a;
	vct.b = *b;
	// ft_rotate(&vct.a, map);
	// ft_rotate(&vct.b, map);
	vct.vert = abs((int)(vct.b.y - vct.a.y)) > abs((int)(vct.b.x - vct.a.x));
	// if (!ft_in_screen(&vct.a, &vct.b))
	// 	return ;
	vct.step = ft_get_step(&vct);
	ft_draw_line(&vct, img, color);
	// int dx;
    // int dy;
    // int steps;
    // int i;
    // float x_inc;
    // float y_inc;
    // float x;
    // float y;

    // dx = b->x - a->x;
    // dy = b->y - a->y;
    // steps = fmax(abs(dx), abs(dy));
    // x_inc = dx / (float)steps;
    // y_inc = dy / (float)steps;
    // x = a->x;
    // y = a->y;
    // i = 0;
    // while (i <= steps)
    // {
    //     ft_put_pixel(x, y, color, img);
    //     x += x_inc;
    //     y += y_inc;
    //     i++;
    // }
}
