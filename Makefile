CC = cc
FLAGS = -Wall -Wextra -Werror -g -fsanitize=address
INCLUDES = -I ./includes -I ./libft -I ./mlx -I ./gnl
HEADERS = includes/cub3d.h
GNL_FILES = gnl/get_next_line.c gnl/get_next_line_utils.c
LIBFT = libft/libft.a
NAME = cub3d

SRC_DIR = src/
OBJ_DIR = obj/

RAW_FILES = main.c
FILES = $(addprefix $(SRC_DIR), $(RAW_FILES))
OBJ = $(addprefix $(OBJ_DIR), $(RAW_FILES:.c=.o))

all: libft $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

libft:
	@make -C libft

$(NAME): $(OBJ) $(HEADERS)
	@$(CC) $(FLAGS) $(INCLUDES) -o $(NAME) $(OBJ) $(LIBFT) $(GNL_FILES) -lmlx -framework OpenGL -framework AppKit

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

.PHONY: all libft clean fclean re
