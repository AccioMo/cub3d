


#ifndef PARSE_H
# define PARSE_H

# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <math.h>
# include <stdio.h>
# include <sys/time.h>
# include "libft.h"
# include "mlx.h"
# include "get_next_line.h"

# define W_WIDTH 1920
# define W_HEIGHT 1080

# define TEXTURE_HEIGHT 64
# define TEXTURE_WIDTH 64

# define MOUSE_LEFT 1
# define MOUSE_RIGHT 2
# define MOUSE_MIDDLE 3
# define MOUSE_SCROLL_UP 4
# define MOUSE_SCROLL_DOWN 5

# define ON_KEYDOWN 2
# define ON_KEYUP 3
# define ON_MOUSEDOWN 4
# define ON_MOUSEUP 5
# define ON_MOUSEMOVE 4
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
# define KEY_R_SHIFT 257
# define KEY_L_SHIFT 258

# define DIR_LEN 50
# define MOVE_SPEED 0.10
# define ROTATION_SPEED 0.12
# define MINIMAP_BLOCKS 8
# define BLOCK_SIZE 24
# define MINIMAP_SIZE MINIMAP_BLOCKS * BLOCK_SIZE
# define CURSOR_SIZE 16

# define MINIMAP_WIDTH 200
# define MINIMAP_HEIGHT 200
# define MAPCHARS "01NSEW"

typedef struct s_image
{
	void	*img;
	char	*addr;
	int		bpp;
	int		n_bytes;
	int		endian;
}	t_image;

typedef struct s_block
{
	int		x;
	int		y;
	int		width;
	int		height;
}	t_block;

typedef enum e_face
{
  NONE,
  NORTH,
  SOUTH,
  WEST,
  EAST,
  HOR_DOOR_CLOSED,
  HOR_DOOR_OPEN,
  VER_DOOR_CLOSED,
  VER_DOOR_OPEN
}	t_face;

typedef enum e_movement
{
	M_NONE,
	M_FORWARD,
	M_BACKWARD,
	M_LEFT,
	M_RIGHT
}	t_movement;

typedef enum e_rotation
{
	R_NONE,
	R_LEFT,
	R_RIGHT
}	t_rotation;

typedef struct s_point
{
	double	x;
	double	y;
}	t_point;

typedef struct s_line
{
	t_point	a;
	t_point	b;
	float	dx;
	int		rev;
	int		vert;
	float	step;
}	t_line;

typedef struct s_mouse
{
	int	x;
	int	y;
	int	click;
	double	acceleration;
	t_rotation	rotate;
}	t_mouse;

typedef struct s_map
{
	int		width;
	int		height;
	int		x_offset;
	int		y_offset;
	int		x;
	int		y;
	char	*name;
	char	**blocks;
}	t_map;

typedef struct e_ray
{
	t_point	dir;
	t_point	side;
	t_point	delta;
	t_point	hit;
	t_point	tex;
	t_face		face;
	double		perp_wall_dist;
	double		wall_x;
	double		step;
	int			draw_start;
	int			draw_end;
}	t_ray;

typedef struct s_player
{
	float		x;
	float		y;
	int			angle;
	char		*target;
	t_movement	move;
	t_rotation	rotate;
}	t_player;

typedef struct s_textures
{
	void	*north;
	void	*south;
	void	*east;
	void	*west;
	void	*floor;
	void	*ceiling;
	void	*closed_door;
	void	*open_door;
	void	*sprite_frames[3];
}	t_textures;

typedef struct e_sprite
{
	double	x;
	double	y;
	int		ds_x;
	int		ds_y;
	int		de_x;
	int		de_y;
	int		sprite_width;
	int		sprite_height;
}	t_sprite;


typedef struct s_env
{
	void		*mlx;
	void		*win;
	t_mouse		mouse;
	t_image		img;
	t_image		mapimg;
	t_image		minimap;
	t_map		map;
	t_player	player;
	t_textures	textures;
	t_sprite	sp;
	t_point		dir;
	t_point		plane;
	char		*no_path;
	char		*so_path;
	char		*we_path;
	char		*ea_path;
	int			floor_color;
	int			ceiling_color;
}	t_env;

/* parse */
void	init_map(t_env *env, char **lines);
void	trim_lines(char **lines);
char	**get_lines(t_env *env);
void	trim_empty_lines(char **lines);
void	parse_params(t_env *env);
void	check_map_lines(char **lines);
int		check_rgb_string(char **rgb);
void	check_if_start_of_map(char *line);
void	flood_fill(t_env *env, char **lines, int x, int y);
int		parse_cfcolor(t_env *env, char *line);



/* render */
int		refresh(t_env *env);

/* draw */
void	ft_put_pixel(int x, int y, int color, t_image *img);
void	ft_paint_block(t_block *block, int color, t_image *img);
void	draw_crosshair(t_env *env);

/* texture */
void	prepare_ray_textures(t_ray *ray);
void	draw_ray_textures(t_ray *ray, int win_x, t_env *env);

int		draw_minimap(t_env *env);
void	render_sprite(double *perp_dist_buffer, int f, t_env *env);

/* mlx */
void	init_mlx(t_env *env);
int		ft_mouse_hook(int code, int x, int y, void *param);
int		ft_mlx_move(t_env *env);
int		ft_keydown_event(int keycode, void *ptr);
int		ft_keyup_event(int keycode, void *ptr);
void	ft_rotate_left(double rotation_speed, t_env *env);
void	ft_rotate_right(double rotation_speed, t_env *env);

/* utils */
double	ft_get_time(void);
int		ft_index(char c, char *str);
int		free_arr(char **arr);
int		arr_len(char **arr);
int		create_trgb(int t, int r, int g, int b);
void	parse_params(t_env *env);
int		ft_mouse_move(int x, int y, void *param);
int		is_wall(char c);
char	prev_block(t_env *env, float *coord);
/* void	draw_circle(int x, int y, int color, t_image *img); <- unused */
/* t_point normalize(t_point *vector); <- unused */

void	dda(t_ray *ray, t_env *env);
//char	*ft_realloc(char *line, char *buffer);


int		get_map_width(char **lines);
void map_to_img(t_env *env, t_image *img);
void mapimg_to_minimap(t_env *env, t_image *mapimg, t_image *minimap);
t_point get_hit_point(t_env *env, t_point hit, t_point rayDir, t_face face);

void toggle_door(t_env *env);
int door_hit(t_env *env, t_ray *ray);
void catigorize_doors(t_env *env);

/* end */
void	ft_exit(int code, char *msg);
int		ft_end(void *param);


void	coord_increase_protection(t_env *env, float *coord, double dirVec_coord);
void	coord_decrease_protection(t_env *env, float *coord, double dirVec_coord);

void	init_textures(t_env *env);

#endif
