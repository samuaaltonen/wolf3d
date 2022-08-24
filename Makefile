# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: htahvana <htahvana@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/25 12:54:14 by htahvana          #+#    #+#              #
#    Updated: 2022/08/24 16:35:40 by htahvana         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = wolf3d

CC = gcc

FILES = main.c error.c conf.c app.c events_key.c events_loop.c \
				events_mouse.c events_window.c graphics.c \
				image.c parser.c object_init.c object_render.c object_draw.c \
				door_init.c door_action.c door_render.c object_action.c \
				ui_views.c helper.c wall_raycast.c wall_render.c sky_bloom.c \
				load_checker.c floor.c player.c

LIBFT = ./libft/libft.a
MINILIBX = ./libmlx/libmlx.a

SRC_DIR = ./srcs
SRCS := $(patsubst %, $(SRC_DIR)/%, $(FILES))

OBJ_DIR = ./objs
OBJS = $(patsubst %, $(OBJ_DIR)/%, $(FILES:.c=.o))

HEADERS = -I ./includes -I ./libft/includes

FLAGS = -Wall -Wextra -Werror -O3

LIBLINKS = -I ./libft/includes -L ./libft -lft

MACLINKS = $(HEADERS) \
	-I ./libmlx -L ./libmlx \
	-lmlx -framework OpenGL -framework Appkit

LLINKS = $(HEADERS) -Lmlx_linux -lmlx_Linux -lX11 -lXext -lm -lz -lpthread

all: $(NAME)

$(NAME): $(MINILIBX) $(LIBFT) $(OBJS)
	$(CC) $(OBJS) -o $(NAME) $(FLAGS) $(LIBLINKS) $(MACLINKS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(FLAGS) $(HEADERS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(LIBFT):
	make -C ./libft

echo:
	echo $(OBJS)

.PHONY: all clean fclean linux

clean:
	make clean -C ./libft
	make clean -C ./libmlx
	/bin/rm -rf $(OBJ_DIR)

fclean: clean
	make fclean -C ./libft
	/bin/rm -f $(NAME)

re: fclean all

$(MINILIBX):
	make -C ./libmlx

mac: fclean $(NAME)

linux:
	make reclean -C ./libft
	$(CC) $(SRCS) -o $(NAME) $(FLAGS) $(LIBLINKS) $(LLINKS)
