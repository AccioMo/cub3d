void draw_map(t_env *env)
{
	t_map *map = &env->map;
	t_block	block;
	int h = env->player.y / BLOCK_SIZE - 4 > 0 ? env->player.y / BLOCK_SIZE - 4 : 0;
	int w = env->player.x / BLOCK_SIZE - 4 > 0 ? env->player.x / BLOCK_SIZE - 4 : 0;
	int max_h = env->player.y / BLOCK_SIZE + 4 < map->height ? env->player.y / BLOCK_SIZE + 4 : map->height;
	int max_w = env->player.x / BLOCK_SIZE + 4 < map->width ? env->player.x / BLOCK_SIZE + 4 : map->width;

	int j;
	int i = 0;
	while (h + i < max_h)
	{
		j = 0;
		while (w + j < max_w)
		{
			block = (t_block){j * BLOCK_SIZE, i * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
			ft_paint_block(&block, map->blocks[h + i][w + j] * 0xFFFFFF, env);
			j++;
		}
		i++;
	}
}