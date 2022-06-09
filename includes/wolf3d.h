/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 14:20:36 by saaltone          #+#    #+#             */
/*   Updated: 2022/06/09 13:20:38 by saaltone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF3D_H
# define WOLF3D_H
# define WIN_NAME "Wolf 3D"
# define WIN_W 1920
# define WIN_H 1080
# define MSG_ERROR "Error occured"
# define MSG_ERROR_WINDOW "Could not open a window."
# define MSG_ERROR_ALLOC "Could not allocate memory."
# define MSG_ERROR_IMAGE_INIT "Image initialization failed."
# define MSG_ERROR_THREADS "Could not create a thread."
# define MSG_ERROR_MAP_FILE_ACCESS "Could not open/close map file."
# define MSG_ERROR_MAP_INVALID "Map file is invalid."
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
# define MAP_FILE "map.txt"
# define MAP_WIDTH 20
# define MAP_HEIGHT 20
# define POSITION_START_X 10.f
# define POSITION_START_Y 12.f
# define DIRECTION_START_X 1.f
# define DIRECTION_START_Y 0.f
# define CAMERA_PLANE_X 0.f
# define CAMERA_PLANE_Y 0.66f
# define ROTATION 0.01745329238f
# include <fcntl.h>
# include <stdio.h>
# include <math.h>
# include <time.h>
# include <pthread.h>
# include <stdlib.h>
# include "libft.h"
# include "mlx.h"

typedef struct vector2d {
	double	x;
	double	y;
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

typedef struct s_player
{
	t_vector2d	position;
	t_vector2d	direction;
	t_vector2d	camera_plane;
}	t_player;

typedef struct s_app
{
	t_conf			*conf;
	t_image			*image;
	void			*mlx;
	void			*win;
	t_thread_data	thread_info[THREADS_MAX];
	int				map[MAP_HEIGHT][MAP_WIDTH];
	t_player		player;
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

/*
 * Graphics
*/
int		raycast(t_app *app, int x, double *distance);
void	render_view(t_app *app);

/*
 * Map
*/
int		parse_map(t_app *app);

#endif
