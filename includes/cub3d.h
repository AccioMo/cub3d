/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzeggaf <mzeggaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 20:01:47 by mzeggaf           #+#    #+#             */
/*   Updated: 2024/10/04 16:59:44 by mzeggaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

# define BLOCK_SIZE 69

# define UPPER_HEX "0123456789ABCDEF"
# define LOWER_HEX "0123456789abcdef"

# define ISOMETRIC "Isometric"
# define ELEVATION "Elevation"
# define PLAN "Plan"

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
	int		size;
	int		wall;
}	t_block;

typedef struct s_map
{
	int		width;
	int		height;
	char	*name;
	t_block	**blocks;
}	t_map;

typedef struct s_player
{
	int		x;
	int		y;
	int		angle;
}	t_player;

typedef struct s_env
{
	void		*mlx;
	void		*win;
	t_mouse		mouse;
	t_image		img;
	t_map		map;
	t_player	player;
}	t_env;


#endif
