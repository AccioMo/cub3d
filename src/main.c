/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzeggaf <mzeggaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 22:35:25 by mzeggaf           #+#    #+#             */
/*   Updated: 2024/12/02 23:51:52 by mzeggaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	f(void)
{
	system("leaks cub3d");
}

static void	init_params(char *map_name, t_env *env)
{
	env->map.name = map_name;
	env->no_path = NULL;
	env->so_path = NULL;
	env->we_path = NULL;
	env->ea_path = NULL;
	env->floor_color = 0;
	env->ceiling_color = 0x808080;
	env->player.move = M_NONE;
	env->player.rotate = R_NONE;
}

static void	init_player(t_env *env)
{
	env->dir.x = 0;
	env->dir.y = -1;
	env->plane.x = 0.66f;
	env->plane.y = 0;
	if (env->player.angle == 1)
	{
		env->dir.x = 1;
		env->dir.y = 0;
		env->plane.x = 0;
		env->plane.y = 0.66f;
	}
	else if (env->player.angle == 2)
	{
		env->dir.x = 0;
		env->dir.y = 1;
		env->plane.x = -0.66f;
		env->plane.y = 0;
	}
	else if (env->player.angle == 3)
	{
		env->dir.x = -1;
		env->dir.y = 0;
		env->plane.x = 0;
		env->plane.y = -0.66f;
	}
}

int	main(int ac, char **av)
{
	t_env	env;

	if (ac != 2)
	{
		ft_putstr_fd("usage: ./cub3d [map].cub\n", 2);
		return (1);
	}
	// atexit(f);
	if (ft_strncmp(av[1] + ft_strlen(av[1]) - 4, ".cub", 4))
		ft_exit(0, "invalid file extension\n");
	init_params(av[1], &env);
	parse_params(&env);
	init_player(&env);
	catigorize_doors(&env);
	init_mlx(&env);
	mlx_mouse_hide();
	mlx_hook(env.win, ON_KEYDOWN, 0, ft_keydown_event, (void *)&env);
	mlx_hook(env.win, ON_KEYUP, 0, ft_keyup_event, (void *)&env);
	mlx_hook(env.win, ON_DESTROY, 0, ft_end, (void *)&env);
	mlx_loop_hook(env.mlx, refresh, (void *)&env);
	mlx_hook(env.win, 6, 0, ft_mouse_move, (void *)&env);
	mlx_mouse_hook(env.win, ft_mouse_hook, &env);
	mlx_loop(env.mlx);
}
