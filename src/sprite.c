/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzeggaf <mzeggaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 19:37:39 by mzeggaf           #+#    #+#             */
/*   Updated: 2024/12/02 23:39:40 by mzeggaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_sprite(int sprite_screen_x, int f, t_env *env)
{
	unsigned int	color;
	int				tex_x;
	int				tex_y;
	int				b;
	int				d;

	tex_x = (int)(256 * (env->sp.ds_x - (-env->sp.sprite_width / 2 + \
		sprite_screen_x)) * TEXTURE_WIDTH / env->sp.sprite_width) / 256;
	b = env->sp.ds_y;
	while (env->sp.ds_y < env->sp.de_y)
	{
		d = (env->sp.ds_y) * 256 - W_HEIGHT * 128 + \
			env->sp.sprite_height * 128;
		tex_y = ((d * TEXTURE_HEIGHT) / env->sp.sprite_height) / 256;
		color = ((unsigned int *)env->textures.sprite_frames[f] \
			)[TEXTURE_WIDTH * tex_y + tex_x];
		if ((color & 0x00FFFFFF) != 0)
		{
			ft_put_pixel(env->sp.ds_x, env->sp.ds_y, \
				color, &env->img);
		}
		env->sp.ds_y++;
	}
	env->sp.ds_y = b;
}

t_point	get_transform(int *sprite_screen_x, t_env *env)
{
	t_point	transform;
	double	sprite_x;
	double	sprite_y;
	double	inv_d;

	sprite_x = env->sp.x - env->player.x;
	sprite_y = env->sp.y - env->player.y;
	inv_d = 1.0 / (env->plane.x * env->dir.y - env->dir.x * env->plane.y);
	transform.x = inv_d * (env->dir.y * sprite_x - env->dir.x * sprite_y);
	transform.y = inv_d * (-env->plane.y * sprite_x + env->plane.x * sprite_y);
	*sprite_screen_x = (int)((W_WIDTH / 2) * (1 + transform.x / transform.y));
	return (transform);
}

void	render_sprite(double *perp_dist_buffer, int f, t_env *env)
{
	t_point	transform;
	int		sprite_screen_x;

	transform = get_transform(&sprite_screen_x, env);
	env->sp.sprite_height = abs((int)(W_HEIGHT / (transform.y)));
	env->sp.ds_y = -env->sp.sprite_height / 2 + W_HEIGHT / 2;
	if (env->sp.ds_y < 0)
		env->sp.ds_y = 0;
	env->sp.de_y = env->sp.sprite_height / 2 + W_HEIGHT / 2;
	if (env->sp.de_y >= W_HEIGHT)
		env->sp.de_y = W_HEIGHT - 1;
	env->sp.sprite_width = abs((int)(W_HEIGHT / transform.y));
	env->sp.ds_x = -env->sp.sprite_width / 2 + sprite_screen_x;
	if (env->sp.ds_x < 0)
		env->sp.ds_x = 0;
	env->sp.de_x = env->sp.sprite_width / 2 + sprite_screen_x;
	if (env->sp.de_x >= W_WIDTH)
		env->sp.de_x = W_WIDTH - 1;
	while (env->sp.ds_x < env->sp.de_x)
	{
		if (transform.y > 0 && env->sp.ds_x > 0 && env->sp.ds_x < W_WIDTH \
			&& transform.y < perp_dist_buffer[env->sp.ds_x])
			draw_sprite(sprite_screen_x, f, env);
		env->sp.ds_x++;
	}
}
