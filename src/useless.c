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

