CC = cc
FLAGS = -Wall -Wextra -Werror -O3
INCLUDES = -I ./includes -I ./libft -I ./mlx -I ./gnl
HEADERS = includes/cub3d.h
GNL_FILES = gnl/get_next_line.c gnl/get_next_line_utils.c
LIBFT = libft/libft.a
MLX_DIR = mlx/
MLX = $(MLX_DIR)libmlx.a
NAME = cub3d

SRC_DIR = src/
OBJ_DIR = obj/

RAW_FILES = main.c draw.c mlx.c parse.c end.c utils.c minimap.c checks.c \
			door.c sprite.c render.c texture.c algorithm.c parse_utils.c \
			mlx_move.c move_protection.c init_textures.c input.c move_utils.c \
			door_checks.c
FILES = $(addprefix $(SRC_DIR), $(RAW_FILES))
OBJ = $(addprefix $(OBJ_DIR), $(RAW_FILES:.c=.o))

all: libft $(MLX) $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

libft:
	@make -C libft

$(MLX): $(MLX_DIR)
	@make -C $<

$(NAME): $(OBJ) $(HEADERS)
	@$(CC) $(FLAGS) $(INCLUDES) -o $(NAME) $(OBJ) $(LIBFT) $(MLX) $(GNL_FILES) -framework OpenGL -framework AppKit

$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(HEADERS)
	@$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	@make -C libft clean

fclean: clean
	rm -f $(NAME)
	@make -C libft fclean

re: fclean all

.PHONY: all libft $(MLX) clean fclean re
