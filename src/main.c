
#include "cub3d.h"

// char	**read_map(char *filename)
// {
// 	int		fd;
// 	char	*line;
// 	char	**map;

// 	fd = open(filename, O_RDONLY);
// 	if (fd < 0)
// 	{
// 		ft_putstr_fd("error opening file.\n", 2);
// 		return (NULL);
// 	}
// 	while (true)
// 	{
// 		line = get_next_line(fd);
// 		if (!line)
// 			break ;
// 		free(line);
// 		if (!map)
// 			break ;
// 	}
// }

void ft_exit(int code, char *msg)
{
	ft_putstr_fd("Error\n", 2);
	if (code == 1)
		ft_putstr_fd("Error opening file: ", 2);
	else if (code == 2)
		ft_putstr_fd("Invalid file extension: ", 2);
	else if (code == 3)
		ft_putstr_fd("Error allocating memory: ", 2);
	else if (code == 4)
		ft_putstr_fd("Error reading file: ", 2);
	else if (code == 5)
		ft_putstr_fd("mlx: ", 2);
	ft_putstr_fd(msg, 2);
	exit(EXIT_FAILURE);
}

void init_mlx(t_env *env)
{
	env->mlx = mlx_init();
	if (!env->mlx)
		ft_exit(5, "init error.\n");
	env->win = mlx_new_window(env->mlx, W_WIDTH, W_HEIGHT, "Fil de Fer");
	if (!env->win)
		ft_exit(5, "error creating window.\n");
	env->img.img = mlx_new_image(env->mlx, W_WIDTH, W_HEIGHT);
	if (!env->img.img)
		ft_exit(5, "error creating image.\n");
	env->img.addr = mlx_get_data_addr(env->img.img, &env->img.bpp,
									  &env->img.n_bytes, &env->img.endian);
	if (!env->img.addr)
		ft_exit(5, "error initializing image data.\n");
}

int ft_index(char c, char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (c == str[i])
			return (i);
		i++;
	}
	return (i);
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

int ft_get_pixel_color(int x, int y, t_image *img)
{
	char *dst;

	if ((0 <= x && x < W_WIDTH) && (0 <= y && y < W_HEIGHT))
	{
		x *= (img->bpp / 8);
		y *= img->n_bytes;
		dst = img->addr + x + y;
		return (*(unsigned int *)dst);
	}
	return (1);
}

void ft_paint_block(t_block *block, t_env *env)
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
			// 	ft_put_pixel(w + block->x, h + block->y, 0xFF0000, &env->img);
			// else
				ft_put_pixel(w + block->x, h + block->y, block->color * block->wall, &env->img);
			w += 1;
		}
		h += 1;
	}
}

void draw_map(t_env *env)
{
	t_map *map = &env->map;
	int h = env->player.y / BLOCK_SIZE;
	int w = env->player.x / BLOCK_SIZE;

	while (h < map->height)
	{
		w = 0;
		while (w < map->width)
		{
			ft_paint_block(&map->blocks[h][w], env);
			w += 1;
		}
		h += 1;
	}
}

void draw_circule(int x, int y, int color, t_env *env)
{
	double i = 0;
	double angle;
	double x1;
	double x2;
	double y1;
	double r = 5;

	while (i < 360)
	{
		angle = i;
		x1 = r * cos(angle * M_PI / 180);
		y1 = r * sin(angle * M_PI / 180);
		x2 = round(x + x1);
		while (x2 != round(x))
		{
			ft_put_pixel(x2, round(y + y1), color, &env->img);
			if (x2 < round(x))
				x2 += 1;
			else
				x2 -= 1;
		}
		ft_put_pixel(x2, round(y + y1), color, &env->img);
		i += 0.1f;
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

	while (i < 360)
	{
		angle = i;
		x1 = r * cos(angle * M_PI / 180);
		y1 = r * sin(angle * M_PI / 180);
		x2 = round(env->player.x + x1);
		while (x2 != round(env->player.x))
		{
			ft_put_pixel(x2, round(env->player.y + y1), 0xFFFFFF, &env->img);
			if (x2 < round(env->player.x))
				x2 += 1;
			else
				x2 -= 1;
		}
		ft_put_pixel(x2, round(env->player.y + y1), 0xFFFFFF, &env->img);
		i += 0.1f;
	}
}

int draw_direction(t_env *env)
{
	// initial direction
	t_point x = {env->player.x, env->player.y, 0xFFFFFF};
	t_point y = {env->player.x + (1 * env->dirVec.x), env->player.y + (1 * env->dirVec.y), 0xFFFFFF};

	ft_aa_draw(&x, &y, &env->img, 0xFF0000);
	return (0);
}

double vector_length(t_vector_ a)
{
	return (sqrt(a.x * a.x + a.y * a.y));
}

void convert_to_unit_vector(t_vector_ *u)
{
	u->x = u->x / vector_length(*u);
	u->y = u->y / vector_length(*u);
}

int point_in_wall(t_point p, t_env *env, t_hitdir hit, t_vector_ *rayDir)
{
	(void)hit;
	(void)rayDir;
	/*
	if (hit == HORIZONTAL)
	  p.x += rayDir->x < 0 ? -(BLOCK_SIZE / 2) : (BLOCK_SIZE / 2);
	else if (hit == VERTICAL)
	  p.y += rayDir->y < 0 ? (BLOCK_SIZE / 2) : -(BLOCK_SIZE / 2);
	*/

	int x = (int)(p.x / BLOCK_SIZE);
	int y = (int)(p.y / BLOCK_SIZE);
	return (env->map.blocks[y][x].wall);
}

// given the wall coordinates, return the point in wall that ray hits
t_point get_hit_point(t_env *env, t_hitwall hit, t_vector_ rayDir)
{
	int x;
	int y;
	t_point result;

	x = abs(hit.x * BLOCK_SIZE - env->player.x) < abs((hit.x + 1) * BLOCK_SIZE - env->player.x) ? hit.x * BLOCK_SIZE : (hit.x + 1) * BLOCK_SIZE;
	y = abs(hit.y * BLOCK_SIZE - env->player.y) < abs((hit.y + 1) * BLOCK_SIZE - env->player.y) ? hit.y * BLOCK_SIZE : (hit.y + 1) * BLOCK_SIZE;

	double m = rayDir.y / rayDir.x;
	double c = env->player.y - m * env->player.x;
	if (hit.side == 0)
	{
		result.x = x;
		result.y = m * x + c;
	}
	else
	{
		result.y = y;
		result.x = (y - c) / m;
	}
	return (result);
}

t_hitwall dda(t_env *env, double deltax, double deltay, t_vector_ rayDir, t_point *side)
{
	t_hitwall current;
	int stepx;
	int stepy;

	current.x = env->player.x / BLOCK_SIZE;
	current.y = env->player.y / BLOCK_SIZE;

	stepx = rayDir.x < 0 ? -1 : 1;
	stepy = rayDir.y < 0 ? -1 : 1;

	while (1)
	{
		if (side->x < side->y)
		{
			side->x += deltax;
			current.x += stepx;
			current.side = 0;
		}
		else
		{
			side->y += deltay;
			current.y += stepy;
			current.side = 1;
		}
		if ((int)current.y >= env->map.height || \
			(int)current.x >= env->map.width || \
			(int)current.x < 0 || (int)current.y < 0 || \
			env->map.blocks[(int)current.y][(int)current.x].wall)
			return (current);
	}
}

int draw_minimap(t_env *env)
{
	t_point player = {env->player.x, env->player.y, 0xFFFFFF};

	draw_map(env);
	draw_player(env);
	for (int i = 0; i < W_WIDTH; i++)
	{
		double windowX = 2 * i / (double)W_WIDTH - 1;
		t_vector_ rayDir = {env->dirVec.x + env->planeVec.x * windowX, env->dirVec.y + env->planeVec.y * windowX};

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

		t_point side = {initstepx, initstepy, 0x008000};
		t_hitwall wall = dda(env, deltax, deltay, rayDir, &side);
		t_point x = get_hit_point(env, wall, rayDir);
		ft_aa_draw(&player, &x, &env->img, 0x008000);
	}
	return (0);
}

int draw_camera_plane(t_env *env)
{
	int w = 1;
	int h = W_HEIGHT;
	ft_paint_block(&(t_block){0, 0, W_HEIGHT, W_WIDTH, 0, 0}, env);
	for (int i = 0; i < (W_WIDTH); i++)
	{
		double windowX = 2 * i / (double)(W_WIDTH) - 1;
		t_vector_ rayDir = {env->dirVec.x + env->planeVec.x * windowX, env->dirVec.y + env->planeVec.y * windowX};

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

		t_point side = {initstepx, initstepy, 0x008000};
		t_hitwall wall = dda(env, deltax, deltay, rayDir, &side);

		double perpWallDist;
		if (wall.side == 0)
			perpWallDist = (((side.x - deltax) / deltax) / fabs(rayDir.x));
		else
			perpWallDist = (((side.y - deltay) / deltay) / fabs(rayDir.y));

		int lineHeight = ((int)(h / perpWallDist));

		int drawStart = -lineHeight / 2 + h / 2;
		if (drawStart < 0)
			drawStart = 0;
		int drawEnd = lineHeight / 2 + h / 2;
		if (drawEnd >= h)
			drawEnd = h - 1;

		t_block block;
		block.x = (i * w);
		block.y = drawStart;
		block.height = (drawEnd - drawStart);
		block.width = w;
		block.color = 0xFFFFFF;
		if (wall.side == 1)
			block.color = 0xdddddd;
		block.wall = 1;
		ft_paint_block(&block, env);
	}
	return (0);
}

void refresh(t_env *env)
{
	draw_camera_plane(env);
	draw_minimap(env);
	mlx_put_image_to_window(env->mlx, env->win, env->img.img, 0, 0);
}

t_block *parse_line(char *line, t_env *env, int h)
{
	t_map *map = &env->map;
	t_block *blocks;
	int i;
	if (map->width == 0)
		map->width = ft_strlen(line);
	else if (map->width != (int)ft_strlen(line))
		ft_exit(5, "invalid map.\n");
	blocks = (t_block *)malloc((map->width + 1) * sizeof(t_block));
	i = 0;
	while (i < map->width)
	{
		blocks[i].x = i * BLOCK_SIZE;
		blocks[i].y = h * BLOCK_SIZE;
		blocks[i].height = BLOCK_SIZE;
		blocks[i].width = BLOCK_SIZE;
		if (line[i] == '1')
			blocks[i].wall = 1;
		else if (line[i] == '0')
			blocks[i].wall = 0;
		else if (ft_index(line[i], "NESW") < 4)
		{
			env->player.x = blocks[i].x + BLOCK_SIZE / 2;
			env->player.y = blocks[i].y + BLOCK_SIZE / 2;
			env->player.angle = ft_index(line[i], "NESW");
			blocks[i].wall = 0;
		}
		i++;
	}
	return (blocks);
}

void init_map(t_env *env)
{
	char *line;
	int fd;
	int h;

	env->map.width = 0;
	env->map.height = 0;
	fd = open(env->map.name, O_RDONLY);
	if (fd < 0)
		ft_exit(1, env->map.name);
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break;
		free(line);
		env->map.height++;
	}
	close(fd);
	printf("height: %d\n", env->map.height);
	env->map.blocks = (t_block **)malloc(env->map.height * sizeof(t_block *));
	fd = open(env->map.name, O_RDONLY);
	if (fd < 0)
		ft_exit(1, env->map.name);
	h = 0;
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break;
		env->map.blocks[h] = parse_line(line, env, h);
		free(line);
		h++;
	}
}

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
	draw_direction(env);
	draw_camera_plane(env);
	mlx_put_image_to_window(env->mlx, env->win, env->img.img, 0, 0);
	return (1);
}

void ft_free(void **ptr)
{
	void **hold;

	hold = ptr;
	if (!ptr)
		return;
	while (*ptr)
		free(*ptr++);
	free(hold);
}

int ft_end(void *param)
{
	t_env *env;

	env = (t_env *)param;
	mlx_clear_window(env->mlx, env->win);
	mlx_destroy_image(env->mlx, env->img.img);
	mlx_destroy_window(env->mlx, env->win);
	// ft_free((void **)env->map.blocks);
	exit(0);
	return (1);
}

int ft_mlx_move(int keycode, t_env *env)
{
	if (keycode == KEY_A)
	{
		env->player.x -= 5;
		refresh(env);
		return (0);
	}
	else if (keycode == KEY_D)
	{
		env->player.x += 5;
		refresh(env);
		return (0);
	}
	else if (keycode == KEY_W)
	{
		env->player.y -= 5;
		refresh(env);
		return (0);
	}
	else if (keycode == KEY_S)
	{
		env->player.y += 5;
		refresh(env);
		return (0);
	}
	return (1);
}

int ft_key_event(int keycode, void *ptr)
{
	t_env *env;
	double rotSpeed;

	rotSpeed = 0.05;
	env = (t_env *)ptr;
	if (keycode == ESC)
		ft_end(env);
	else if (keycode == KEY_RIGHT)
	{
		double oldDirX = env->dirVec.x;
		env->dirVec.x = env->dirVec.x * cos(-rotSpeed) - env->dirVec.y * sin(-rotSpeed);
		env->dirVec.y = oldDirX * sin(-rotSpeed) + env->dirVec.y * cos(-rotSpeed);
		double oldPlaneX = env->planeVec.x;
		env->planeVec.x = env->planeVec.x * cos(-rotSpeed) - env->planeVec.y * sin(-rotSpeed);
		env->planeVec.y = oldPlaneX * sin(-rotSpeed) + env->planeVec.y * cos(-rotSpeed);
		refresh(env);
	}
	else if (keycode == KEY_LEFT)
	{
		double oldDirX = env->dirVec.x;
		env->dirVec.x = env->dirVec.x * cos(rotSpeed) - env->dirVec.y * sin(rotSpeed);
		env->dirVec.y = oldDirX * sin(rotSpeed) + env->dirVec.y * cos(rotSpeed);
		double oldPlaneX = env->planeVec.x;
		env->planeVec.x = env->planeVec.x * cos(rotSpeed) - env->planeVec.y * sin(rotSpeed);
		env->planeVec.y = oldPlaneX * sin(rotSpeed) + env->planeVec.y * cos(rotSpeed);
		refresh(env);
	}
	else if (!ft_mlx_move(keycode, env))
		return (0);
	return (0);
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
	env.dirVec.x = -1;
	env.dirVec.y = -1;
	env.planeVec.x = env.dirVec.x;
	env.planeVec.y = -env.dirVec.y;
	init_map(&env);
	init_mlx(&env);
	draw_map(&env);
	draw_player(&env);
	draw_direction(&env);
	draw_camera_plane(&env);

	mlx_hook(env.win, ON_KEYDOWN, 0, ft_key_event, (void *)&env);
	mlx_hook(env.win, ON_DESTROY, 0, ft_end, (void *)&env);
	// mlx_hook(env.win, ON_MOUSEMOVE, 0, ft_mouse_move, (void *)&env);
	mlx_put_image_to_window(env.mlx, env.win, env.img.img, 0, 0);

	mlx_loop(env.mlx);
}
