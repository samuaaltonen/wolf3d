# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/16 14:22:44 by saaltone          #+#    #+#              #
#    Updated: 2022/08/05 11:39:19 by saaltone         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=wolf3d
COMP_FLAGS=-Wall -Wextra -Werror -O3 -g
LIBFT=./libft
LIBFT_INCLUDES=./libft/includes
LIBMLX=./libmlx
LIBMLX_INCLUDES=./libmlx
LIBMLX_LINUX=./libmlx_linux
LIBMLX_LINUX_INCLUDES=./libmlx_linux
SOURCES_FILES=main.c error.c conf.c app.c events_key.c events_loop.c \
				events_mouse.c events_window.c graphics.c \
				image.c parser.c object_init.c object_render.c object_draw.c \
				door_init.c door_action.c door_render.c object_action.c \
				finish.c helper.c wall_render.c sky_bloom.c load_checker.c \
				floor.c player.c 
SOURCES=$(SOURCES_FILES:%.c=srcs/%.c)
OBJECTS=$(SOURCES:.c=.o)
INCLUDES=./includes

$(NAME):
	make -C $(LIBFT)
	make -C $(LIBMLX)
	gcc $(COMP_FLAGS) $(SOURCES) -o $(NAME) \
	-I $(INCLUDES) \
	-I $(LIBFT_INCLUDES) \
	-I $(LIBMLX_INCLUDES) \
	-L $(LIBFT) \
	-L $(LIBMLX) \
	-lft -lmlx -framework OpenGL -framework AppKit

all: $(NAME)

clean:
	/bin/rm -f $(OBJECTS)
	make -C $(LIBFT) clean
	make -C $(LIBMLX) clean

fclean: clean
	/bin/rm -f $(NAME)
	make -C $(LIBFT) fclean

re: fclean all

comp:
	gcc $(COMP_FLAGS) $(SOURCES) -o $(NAME) \
	-I $(INCLUDES) \
	-I $(LIBFT_INCLUDES) \
	-I $(LIBMLX_INCLUDES) \
	-L $(LIBFT) \
	-L $(LIBMLX) \
	-lft -lmlx -framework OpenGL -framework AppKit

linux:
	gcc $(COMP_FLAGS) $(SOURCES) -o $(NAME) \
	-I $(INCLUDES) \
	-I $(LIBFT_INCLUDES) \
	-I $(LIBMLX_LINUX_INCLUDES) \
	-L $(LIBFT) \
	-L $(LIBMLX_LINUX) \
	-lft -Lmlx_linux -lmlx_Linux -L/usr/lib -Imlx_linux -lXext -lX11 -lm -lz \
	-pthread

.PHONY: all clean fclean re comp linux
