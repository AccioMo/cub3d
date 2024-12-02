/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_textures.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzeggaf <mzeggaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 01:49:04 by ayait-el          #+#    #+#             */
/*   Updated: 2024/12/02 23:28:23 by mzeggaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	init_texture(t_env *env, char *path, void **dest)
{
	int		img_width;
	int		img_height;
	void	*ptr;
	int		ptr2;

	img_width = 64;
	img_height = 64;
	ptr = mlx_xpm_file_to_image(env->mlx, path, &img_width, &img_height);
	if (!ptr)
		ft_exit(5, "error loading north textures.\n");
	*dest = mlx_get_data_addr(ptr, &ptr2, &ptr2, &ptr2);
	if (!*dest)
		ft_exit(5, "error parsing north textures.\n");
}

void	init_textures(t_env *env)
{
	init_texture(env, env->no_path, &env->textures.north);
	init_texture(env, env->so_path, &env->textures.south);
	init_texture(env, env->we_path, &env->textures.west);
	init_texture(env, env->ea_path, &env->textures.east);
	init_texture(env, "textures/door-closed.xpm", &env->textures.closed_door);
	init_texture(env, "textures/door-open.xpm", &env->textures.open_door);
	init_texture(env, "textures/frame_1.xpm", &env->textures.sprite_frames[0]);
	init_texture(env, "textures/frame_2.xpm", &env->textures.sprite_frames[1]);
	init_texture(env, "textures/frame_3.xpm", &env->textures.sprite_frames[2]);
}
