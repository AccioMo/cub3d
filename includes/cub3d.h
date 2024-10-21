


#ifndef PARSE_H
# define PARSE_H

# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <math.h>
# include <stdio.h>
# include "libft.h"
# include "mlx.h"
# include "get_next_line.h"

# define W_WIDTH 1920
# define W_HEIGHT 1080

# define MOUSE_LEFT 1
# define MOUSE_RIGHT 2
# define MOUSE_MIDDLE 3
# define MOUSE_SCROLL_UP 4
# define MOUSE_SCROLL_DOWN 5

# define ON_KEYDOWN 2
# define ON_KEYUP 3
# define ON_MOUSEDOWN 4
# define ON_MOUSEUP 5
# define ON_MOUSEMOVE 6
# define ON_EXPOSE 12
# define ON_DESTROY 17

# define ESC 53
# define SPACE 49
# define KEY_W 13
# define KEY_A 0
# define KEY_S 1
# define KEY_D 2
# define KEY_UP 126
# define KEY_DOWN 125
# define KEY_RIGHT 124
# define KEY_LEFT 123
# define KEY_PLUS_ 24
# define KEY_MINUS_ 27
# define KEY_PLUS 69
# define KEY_MINUS 78

# define KEY_1 83
# define KEY_2 84
# define KEY_3 85
# define KEY_4 86
# define KEY_5 87
# define KEY_6 88
# define KEY_7 89
# define KEY_8 91
# define KEY_9 92

# define KEY_E 14
# define KEY_P 35
# define KEY_I 34

# define DIR_LEN 50
# define MOVE_SPEED 5
# define BLOCK_SIZE 30
# define PLANE_LEN 1080
# define MINIMAP_BLOCKS 8
# define MINIMAP_SIZE MINIMAP_BLOCKS * BLOCK_SIZE

typedef struct s_image
{
	void	*img;
	char	*addr;
	int		bpp;
	int		n_bytes;
	int		endian;
}	t_image;

typedef struct s_mouse
{
	int	x;
	int	y;
	int	click;
}	t_mouse;

typedef struct s_block
{
	int		x;
	int		y;
	int		width;
	int		height;
}	t_block;

typedef enum e_hit_direction
{
  HORIZONTAL,
  VERTICAL,
  NONE
} t_hitdir;

typedef struct s_point
{
	float	x;
	float	y;
	int		color;
}	t_point;

typedef struct s_hitwall
{
  int x;
  int y;
  int side;
} t_hitwall;

typedef struct s_vector
{
	t_point	a;
	t_point	b;
	float	dx;
	int		rev;
	int		vert;
	float	step;
}	t_vector;

typedef struct s_map
{
	int		width;
	int		height;
	int		x_offset;
	int		y_offset;
	int		x;
	int		y;
	char	*name;
	int		**blocks;
}	t_map;

typedef struct s_player
{
	int		x;
	int		y;
	int		angle;
}	t_player;

typedef struct s_vector_
{
  double x;
  double y;
} t_vector_;

typedef struct s_env
{
	void		*mlx;
	void		*win;
	t_mouse		mouse;
	t_image		img;
	t_map		map;
	t_player	player;
  t_vector_ dirVec;
  t_vector_ planeVec;
}	t_env;

void	ft_aa_draw(t_point *a, t_point *b, t_image *img, int color);
void	ft_put_pixel(int x, int y, int color, t_image *img);

#endif
