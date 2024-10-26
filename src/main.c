
#include "cub3d.h"

t_point normalize(t_point *vector)
{
	t_point normed;

	double magnitude = sqrt(pow(vector->x, 2) + pow(vector->y, 2));

	if (magnitude < 1e-9)
		return (t_point){0, 0, 0};

	normed.x = (double)vector->x / magnitude;
	normed.y = (double)vector->y / magnitude;
	printf("normed vector: %f, %f\n", normed.x, normed.y);
	return (normed);
}

t_point dda(t_env *env, t_point *delta, t_vector rayDir, t_point *side)
{
	t_point current;
	int stepx;
	int stepy;

	current.x = (env->player.x) / BLOCK_SIZE;
	current.y = (env->player.y) / BLOCK_SIZE;

	stepx = rayDir.x < 0 ? -1 : 1;
	stepy = rayDir.y < 0 ? -1 : 1;
	while (1)
	{
		if (side->x < side->y)
		{
			side->x += delta->x;
			current.x += stepx;
			current.color = 0;
		}
		else
		{
			side->y += delta->y;
			current.y += stepy;
			current.color = 1;
		}
		if ((int)current.y >= env->map.height ||
			(int)current.x >= env->map.width ||
			(int)current.x < 0 || (int)current.y < 0 ||
			env->map.blocks[(int)current.y][(int)current.x])
			return (current);
	}
}

int draw_camera_plane(t_env *env)
{
	// int w = 1;
	int h = W_HEIGHT;
	ft_paint_block(&(t_block){0, 0, W_WIDTH, W_HEIGHT / 2}, 0, env);
	ft_paint_block(&(t_block){0, W_HEIGHT / 2, W_WIDTH, W_HEIGHT / 2}, 0x808080, env);
	for (int i = 0; i < (W_WIDTH); i++)
	{
		double windowX = 2 * i / (double)(W_WIDTH) - 1;
		t_vector rayDir = {env->dirVec.x + env->planeVec.x * windowX, env->dirVec.y + env->planeVec.y * windowX};

		double deltax = (rayDir.x == 0) ? BLOCK_SIZE : fabs(1 / rayDir.x) * (double)BLOCK_SIZE;
		double deltay = (rayDir.y == 0) ? BLOCK_SIZE : fabs(1 / rayDir.y) * (double)BLOCK_SIZE;
		double initstepy;
		double initstepx;
		if (rayDir.x < 0)
			initstepx = deltax * ((env->player.x % BLOCK_SIZE) / (double)BLOCK_SIZE);
		else
			initstepx = deltax * ((BLOCK_SIZE - env->player.x % BLOCK_SIZE) / (double)BLOCK_SIZE);
		if (rayDir.y < 0)
			initstepy = deltay * ((env->player.y % BLOCK_SIZE) / (double)BLOCK_SIZE);
		else
			initstepy = deltay * ((BLOCK_SIZE - env->player.y % BLOCK_SIZE) / (double)BLOCK_SIZE);

		t_point delta = {deltax, deltay, 0x008000};
		t_point side = {initstepx, initstepy, 0x008000};
		t_point wall = dda(env, &delta, rayDir, &side);

		double wallX;
		double perpWallDist;
		if (wall.color == 0) // wall
		{
			perpWallDist = (((side.x - deltax) / deltax) / fabs(rayDir.x));
			wallX = env->player.y + perpWallDist * (rayDir.y);
		}
		else
		{
			perpWallDist = (((side.y - deltay) / deltay) / fabs(rayDir.y));
			wallX = env->player.x + perpWallDist * (rayDir.x);
		}

		wallX -= floor(wallX);

		int texX = (int)(wallX * (double)TEXTURE_WIDTH);
		if (wall.color == 0 && rayDir.x > 0)
			texX = TEXTURE_WIDTH - texX - 1;
		if (wall.color == 1 && rayDir.y < 0)
			texX = TEXTURE_WIDTH - texX - 1;

		int lineHeight = ((int)(h / perpWallDist));
		double step = 1.0 * TEXTURE_HEIGHT / lineHeight;

		int drawStart = -lineHeight / 2 + W_HEIGHT / 2;
		if (drawStart < 0)
			drawStart = 0;
		int drawEnd = lineHeight / 2 + W_HEIGHT / 2;
		if (drawEnd >= W_HEIGHT)
			drawEnd = W_HEIGHT - 1;

		double texPos = (drawStart - W_HEIGHT / 2 + lineHeight / 2) * step;
		for(int y = drawStart; y < drawEnd; y++)
		{
			ft_put_pixel(i, y, 0x000000, &env->img);
		}
		for(int y = drawStart; y < drawEnd; y++)
		{
			unsigned int color;
			// Cast the texture coordinate to integer, and mask with (TEXTURE_HEIGHT - 1) in case of overflow
			int texY = (int)texPos;
			texPos += step;
			//make color darker for y-sides: R, G and B byte each divided through two with a "shift" and an "and"
			if(wall.color == 1)
			{
				color = ((unsigned int *)env->textures.east)[TEXTURE_HEIGHT * texY + texX];
				color = ((color >> 1) & 0x7F7F7F);
			}
			else
			{
				color = ((unsigned int *)env->textures.north)[TEXTURE_HEIGHT * texY + texX];
			}
			ft_put_pixel(i, y, color, &env->img);
		}
	}
	refresh(env);
	return (0);
}

void refresh(t_env *env)
{
	draw_camera_plane(env);
	draw_minimap(env);
	mlx_put_image_to_window(env->mlx, env->win, env->img.img, 0, 0);
}

int ft_mouse_move(int x, int y, void *param)
{
	t_env *env;

	env = (t_env *)param;
	t_point mouse = normalize(&(t_point){x - env->player.x, y - env->player.y, 0});
	env->dirVec.x = mouse.x;
	env->dirVec.y = mouse.y;
	env->planeVec.x = env->dirVec.x;
	env->planeVec.y = -env->dirVec.y;
	draw_map(env);
	draw_player(env);
	draw_camera_plane(env);
	mlx_put_image_to_window(env->mlx, env->win, env->img.img, 0, 0);
	return (1);
}

int main(int ac, char **av)
{
	t_env env;

	if (ac != 2)
	{
		ft_putstr_fd("usage: ./cub3d [map].cub\n", 2);
		return (1);
	}
	env.map.name = av[1];
	init_map(&env);
	init_mlx(&env);
	env.dirVec.x = -1;
	env.dirVec.y = -1;
	env.planeVec.x = env.dirVec.x;
	env.planeVec.y = -env.dirVec.y;
	refresh(&env);

	mlx_hook(env.win, ON_KEYDOWN, 0, ft_key_event, (void *)&env);
	mlx_hook(env.win, ON_DESTROY, 0, ft_end, (void *)&env);
	mlx_loop_hook(env.mlx, draw_camera_plane, (void *)&env);
	// mlx_hook(env.win, ON_MOUSEMOVE, 0, ft_mouse_move, (void *)&env);

	// mlx_loop(env.mlx);
}
