
#include "cub3d.h"

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
			if (w == 0 || h == 0)
				ft_put_pixel(w + block->x, h + block->y, 0x808080, &env->img);
			else
				ft_put_pixel(w + block->x, h + block->y, color, &env->img);
			w += 1;
		}
		h += 1;
	}
}

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
	// if (map_offset.x > BLOCK_SIZE / 2)
	// 	map_offset.x = BLOCK_SIZE - map_offset.x;
	// if (map_offset.y > BLOCK_SIZE / 2)
	// 	map_offset.y = BLOCK_SIZE - map_offset.y;
	printf("x: %d, y: %d\n", env->player.x, env->player.y);
	printf("offx: %f, offy: %f\n", map_offset.x, map_offset.y);
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

	if (start_h + MINIMAP_BLOCKS + 1 < map->height)
		end_h = start_h + MINIMAP_BLOCKS + 1;
	else
	{
		end_h = map->height;
		start_h = map->height - MINIMAP_BLOCKS - 1;
		env->map.y = (start_h + MINIMAP_BLOCKS + 1 - map->height) * BLOCK_SIZE + map_offset.y;
		map_offset.y = 0;
	}
	if (start_w + MINIMAP_BLOCKS + 1 < map->width)
		end_w = start_w + MINIMAP_BLOCKS + 1;
	else
	{
		end_w = map->width;
		start_w = map->width - MINIMAP_BLOCKS - 1;
		env->map.x = (start_h + MINIMAP_BLOCKS + 1 - map->width) * BLOCK_SIZE + map_offset.x;
		map_offset.x = 0;
	}

	int j;
	int i = 0;
	int h = BLOCK_SIZE;
	int w = BLOCK_SIZE;
	block = (t_block){1, 1, MINIMAP_BLOCKS * BLOCK_SIZE, MINIMAP_BLOCKS * BLOCK_SIZE};
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
			printf("x: %d, y: %d\n", x, y);
			block = (t_block){x, y, w, h};
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

double vector_length(t_vector_ a)
{
	return (sqrt(a.x * a.x + a.y * a.y));
}

void convert_to_unit_vector(t_vector_ *u)
{
	u->x = u->x / vector_length(*u);
	u->y = u->y / vector_length(*u);
}

t_point get_hit_point(t_player *player, t_hitwall hit, t_vector_ rayDir)
{
	t_point result;
	int x;
	int y;

	x = abs(hit.x * BLOCK_SIZE - (int)player->x) < abs((hit.x + 1) * BLOCK_SIZE - (int)player->x) ? hit.x * BLOCK_SIZE : (hit.x + 1) * BLOCK_SIZE;
	y = abs(hit.y * BLOCK_SIZE - (int)player->y) < abs((hit.y + 1) * BLOCK_SIZE - (int)player->y) ? hit.y * BLOCK_SIZE : (hit.y + 1) * BLOCK_SIZE;

	double m = rayDir.y / rayDir.x;
	double c = (int)player->y - m * (int)player->x;
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

t_hitwall dda(t_env *env, t_point *delta, t_vector_ rayDir, t_point *side)
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
			side->x += delta->x;
			current.x += stepx;
			current.side = 0;
		}
		else
		{
			side->y += delta->y;
			current.y += stepy;
			current.side = 1;
		}
		if ((int)current.y >= env->map.height || \
			(int)current.x >= env->map.width || \
			(int)current.x < 0 || (int)current.y < 0 || \
			env->map.blocks[(int)current.y][(int)current.x])
			return (current);
	}
}

int draw_minimap(t_env *env)
{
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

		t_point delta = {deltax, deltay, 0x008000};
		t_point side = {initstepx, initstepy, 0x008000};
		t_hitwall wall = dda(env, &delta, rayDir, &side);
		t_point x = get_hit_point(&env->player, wall, rayDir);
		t_player player = {MINIMAP_SIZE / 2, MINIMAP_SIZE / 2, 0};
		if (env->map.x || env->map.y)
		{
			player.x = player.x + env->map.x;
			player.y = player.y + env->map.y;
		}
		x.x -= env->player.x - player.x;
		x.y -= env->player.y - player.y;
		ft_aa_draw(&(t_point){player.x, player.y, 0xFFFFFF}, &x, &env->img, 0x008000);
	}
	return (0);
}

int draw_camera_plane(t_env *env)
{
	int w = 1;
	int h = W_HEIGHT;
	ft_paint_block(&(t_block){0, 0, W_WIDTH, W_HEIGHT}, 0, env);
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

		t_point delta = {deltax, deltay, 0x008000};
		t_point side = {initstepx, initstepy, 0x008000};
		t_hitwall wall = dda(env, &delta, rayDir, &side);

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
		if (wall.side == 1)
			ft_paint_block(&block, 0xDDDDDD, env);
		else
			ft_paint_block(&block, 0xFFFFFF, env);
	}
	return (0);
}

void refresh(t_env *env)
{
	draw_camera_plane(env);
	draw_minimap(env);
	mlx_put_image_to_window(env->mlx, env->win, env->img.img, 0, 0);
}

int *parse_line(char *line, t_env *env, int h)
{
	t_map	*map = &env->map;
	int		*blocks;
	int i;
	
	if (map->width == 0)
		map->width = ft_strlen(line);
	else if (map->width != (int)ft_strlen(line))
		ft_exit(5, "invalid map.\n");
	blocks = (int *)malloc((map->width + 1) * sizeof(int));
	i = 0;
	while (i < map->width)
	{
		if (line[i] == '1')
			blocks[i] = 1;
		else if (line[i] == '0')
			blocks[i] = 0;
		else if (ft_index(line[i], "NESW") < 4)
		{
			env->player.x = BLOCK_SIZE * i;
			env->player.y = BLOCK_SIZE * h;
			env->player.angle = ft_index(line[i], "NESW");
			blocks[i] = 0;
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
	env->map.x_offset = 0;
	env->map.y_offset = 0;
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
	env->map.blocks = (int **)malloc(env->map.height * sizeof(int *));
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
		env->player.x += env->dirVec.x * MOVE_SPEED;
		env->player.y -= env->dirVec.y * MOVE_SPEED;
		refresh(env);
		return (0);
	}
	else if (keycode == KEY_D)
	{
		env->player.x -= env->dirVec.x * MOVE_SPEED;
		env->player.y += env->dirVec.y * MOVE_SPEED;
		refresh(env);
		return (0);
	}
	else if (keycode == KEY_W)
	{
		env->player.x += env->dirVec.x * MOVE_SPEED;
		env->player.y += env->dirVec.y * MOVE_SPEED;
		refresh(env);
		return (0);
	}
	else if (keycode == KEY_S)
	{
		env->player.x -= env->dirVec.x * MOVE_SPEED;
		env->player.y -= env->dirVec.y * MOVE_SPEED;
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
	draw_camera_plane(&env);

	mlx_hook(env.win, ON_KEYDOWN, 0, ft_key_event, (void *)&env);
	mlx_hook(env.win, ON_DESTROY, 0, ft_end, (void *)&env);
	// mlx_hook(env.win, ON_MOUSEMOVE, 0, ft_mouse_move, (void *)&env);
	mlx_put_image_to_window(env.mlx, env.win, env.img.img, 0, 0);

	mlx_loop(env.mlx);
}
