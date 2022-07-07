# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: htahvana <htahvana@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/16 14:22:44 by saaltone          #+#    #+#              #
#    Updated: 2022/07/07 16:16:07 by htahvana         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=wolf3d
COMP_FLAGS=-Wall -Wextra -Werror -O3 -g
LIBFT=./libft
LIBFT_INCLUDES=./libft/includes
LIBMLX=./libmlx
LIBMLX_INCLUDES=./libmlx
SOURCES_FILES=main.c error.c conf.c app.c events_key.c events_loop.c \
				events_mouse.c events_window.c graphics.c raycasting.c \
				image.c parser.c textures.c
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

.PHONY: all clean fclean re comp
