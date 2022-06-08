/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 14:20:36 by saaltone          #+#    #+#             */
/*   Updated: 2022/06/08 16:18:23 by saaltone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF3D_H
# define WOLF3D_H
# define WIN_NAME "Wolf 3D"
# define WIN_W 1280
# define WIN_H 720
# define MSG_ERROR "Error occured"
# define MSG_ERROR_WINDOW "Could not open a window."
# define MSG_ERROR_ALLOC "Could not allocate memory."
# define MSG_ERROR_IMAGE_INIT "Image initialization failed."
# define MSG_ERROR_THREADS "Could not create a thread."
# define KEY_ESC 53
# define KEY_A 0
# define KEY_S 1
# define KEY_H 4
# define KEY_ARROW_UP 126
# define KEY_ARROW_DOWN 125
# define KEY_ARROW_LEFT 123
# define KEY_ARROW_RIGHT 124
# define KEY_PGUP 116
# define KEY_PGDOWN 121
# define MOUSE_CLICK_LEFT 1
# define MOUSE_SCROLL_UP 4
# define MOUSE_SCROLL_DOWN 5
# define THREADS_DEFAULT 8
# define THREADS_MAX 32
# define MAP_WIDTH 10
# define MAP_HEIGHT 10
# define POS_START_X 3
# define POS_START_Y 3
# include <fcntl.h>
# include <stdio.h>
# include <math.h>
# include <time.h>
# include <pthread.h>
# include <stdlib.h>
# include "libft.h"
# include "mlx.h"

typedef struct vector2d {
	int	x;
	int	y;
}	t_vector2d;

typedef struct s_image
{
	int		bpp;
	int		line_size;
	int		width;
	int		height;
	int		endian;
	char	*img;
	char	*data;
}	t_image;

typedef struct s_conf
{
	char		*win_name;
	int			win_w;
	int			win_h;
	int			toggle_help;
	int			fps;
	int			fps_time;
	int			fps_count;
	int			thread_count;
}	t_conf;

typedef struct s_thread_data
{
	void	*app;
	int		x_start;
	int		x_end;
}	t_thread_data;

typedef struct s_app
{
	t_conf			*conf;
	t_image			*image;
	void			*mlx;
	void			*win;
	t_thread_data	thread_info[THREADS_MAX];
	int				**map;
}	t_app;

/*
 * Messages
*/
void	exit_error(char *message);

/*
 * Configuration
*/
int		conf_init(t_app *app);
void	init_thread_info(t_app *app);

/*
 * Application
*/
int		app_init(t_app **app);
void	app_run(t_app *app);
void	app_render(t_app *app);

/*
 * Images
*/
t_image	*init_image(void *mlx, t_conf *conf);
void	put_pixel_to_image(t_image *image, int x, int y, int color);
void	flush_image(t_image *image);

/*
 * Events
*/
int		events_keyup(int keycode, t_app *app);
int		events_keydown(int keycode, t_app *app);
int		events_mouse(int mousecode, int x, int y, t_app *app);
int		events_mouse_track(int x, int y, t_app *app);
int		events_loop(t_app *app);

#endif
