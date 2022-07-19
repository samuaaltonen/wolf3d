/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htahvana <htahvana@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 14:20:36 by saaltone          #+#    #+#             */
/*   Updated: 2022/07/19 14:31:09 by htahvana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF3D_H
# ifdef linux
#  include "keybinds_linux.h"
# else
#  include "keybinds_mac.h"
# endif
# define WOLF3D_H
# define WIN_NAME "Wolf 3D"
# define WIN_W 1280
# define WIN_H 1000
# define MSG_ERROR "Error occured"
# define MSG_ERROR_WINDOW "Could not open a window."
# define MSG_ERROR_ALLOC "Could not allocate memory."
# define MSG_ERROR_IMAGE_INIT "Image initialization failed."
# define MSG_ERROR_THREADS "Could not create a thread."
# define MSG_ERROR_THREADS_JOIN "Could not join threads."
# define MSG_ERROR_MAP_FILE_ACCESS "Could not open/close map file."
# define MSG_ERROR_TEXTURE_FILE_ACCESS "Could not open/close a texture file."
# define MSG_ERROR_MAP_INVALID "Map file is invalid."
# define THREADS_DEFAULT 8
# define THREADS_MAX 32
# define MAP_FILE "./map_quad.txt"
# define MAP_BYTES 4
# define IMAGE_PIXEL_BYTES 4
# define MAP_MAX_OBJECT_IDS 9
# define POSITION_START_X 10.f
# define POSITION_START_Y 12.f
# define DIRECTION_START_X 1.f
# define DIRECTION_START_Y 0
# define FOV 66
# define DEG_IN_RADIAN 0.01745f
# define ROTATION_SPEED 0.03125f
# define MOVEMENT_SPEED 0.0625f
# define TARGET_FRAME_TIME 0.01666f
# define MAX_RAY_DISTANCE 50.f
# define TEX_SIZE 64
# define MOUSE_SENSITIVITY 25.f
# define FPS_UPDATE_FREQUENCY 0.0625f
# define TEXTURE_COIN_SPIN "./assets/coin_spin.xpm"
# define TEXTURE_COIN_WHIRL "./assets/coin_whirl.xpm"
# define TEXTURE_PILLAR "./assets/pillar_64.xpm"
# define TEXTURE_CANNON "./assets/cannon_64.xpm"
# define TEXTURE_PANELS "./assets/texture_spritesheet.xpm"
# include <fcntl.h>
# include <stdio.h>
# include <math.h>
# include <time.h>
# include <pthread.h>
# include <stdlib.h>
# include "libft.h"
# include "mlx.h"

enum e_mlx_events {
	ON_KEYDOWN = 2,
	ON_KEYUP = 3,
	ON_MOUSEDOWN = 4,
	ON_MOUSEUP = 5,
	ON_MOUSEMOVE = 6,
	ON_DESTROY = 17
};

typedef enum e_cardinal {
	NORTH = 0,
	EAST = 1,
	SOUTH = 2,
	WEST = 3
}	t_cardinal;

typedef enum e_movement {
	FORWARD = 0,
	BACKWARD = 1,
	LEFT = 2,
	RIGHT = 3
}	t_movement;

enum e_keystates {
	FORWARD_DOWN = 1,
	FORWARD_W_DOWN = 2,
	BACKWARD_DOWN = 4,
	BACKWARD_S_DOWN = 8,
	LEFT_DOWN = 16,
	RIGHT_DOWN = 32,
	ROTATE_LEFT_DOWN = 64,
	ROTATE_RIGHT_DOWN = 128
};

typedef struct s_point
{
	int	x;
	int	y;
}	t_point;

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
	char	*win_name;
	int		win_w;
	int		win_h;
	int		toggle_help;
	int		fps;
	clock_t	fps_clock;
	int		fps_count;
	double	delta_time;
	int		thread_count;
	int		fov;
	t_point	mouse_position;
	int		keystates;
	double	movement_speed;
	double	rotation_speed;
}	t_conf;

typedef struct s_thread_data
{
	void	*app;
	int		x_start;
	int		x_end;
	int		y_start;
	int		y_end;
	int		id;
}	t_thread_data;

typedef struct s_sprite_data
{
	char		*path;
	t_image		*image;
	double		offset_multiplier;
	int			animation_step;
	int			total_steps;
}	t_sprite_data;

typedef struct s_player
{
	t_vector2	position;
	t_vector2	direction;
	t_vector2	camera_plane;
}	t_player;

typedef struct s_object
{
	t_vector2	position;
	int			sprite_id;
	int			width;
	int			height;
	t_vector2	render_step;
}	t_object;

typedef struct s_app
{
	t_conf			*conf;
	t_image			*image;
	void			*mlx;
	void			*win;
	t_thread_data	thread_info[THREADS_MAX];
	t_point			map_size;
	char			***map;
	t_player		player;
	t_image			*sprite;
	t_object		*objects;
	t_sprite_data	object_sprites[MAP_MAX_OBJECT_IDS];
	int				object_count;
	int				objects_pool_size;
	double			distance_buffer[WIN_W];
}	t_app;

typedef struct s_rayhit
{
	t_cardinal	direction;
	int			type;
	int			tex_x;
	double		distance;
}	t_rayhit;

/*
 * Messages
*/
void		exit_error(char *message);

/*
 * Configuration
*/
int			conf_init(t_app *app);
void		init_thread_info(t_app *app);
void		init_camera_plane(t_app *app);

/*
 * Application
*/
int			app_init(t_app **app);
void		app_run(t_app *app);
void		app_render(t_app *app);

/*
 * Images
*/
t_image		*init_image(void *mlx, int x, int y);
void		put_pixel_to_image(t_image *image, int x, int y, int color);
void		put_pixel_to_image_depth(t_image *image, int x, int y, int color);
void		flush_image(t_image *image);
int			get_pixel_color(t_image *image, int x, int y);
t_image		*init_xpm_image(void *mlx, int width, int height, char *path);

/*
 * Events
*/
int			events_keyup(int keycode, t_app *app);
int			events_keydown(int keycode, t_app *app);
int			events_mouse_down(int mousecode, int x, int y, t_app *app);
int			events_mouse_up(int mousecode, int x, int y, t_app *app);
int			events_mouse_track(int x, int y, t_app *app);
int			events_window_destroy(void);
int			events_loop(t_app *app);

/*
 * Graphics
*/
int			raycast(t_app *app, int x, t_rayhit *rayhit);
int			check_ray_pos(t_app *app, t_vector2 *ray);
void		*render_view(void *data);
void		*render_background(void *data);
void		*render_objects(void *data);
void		render_multithreading(t_app *app, void *(*renderer)(void *));
/*
 * Map
*/
int			parse_map(t_app *app);
int			check_map(t_app *app);

/*
 * Objects
*/
void		draw_object(t_app *app, t_vector2 *transform, int index, int screen_x, int depth);
void		init_objects(t_app *app);
void		load_object_textures(t_app *app);
void		cast_objects(t_app *app);

/**
 * Player
*/
void		player_rotate(t_app *app, double angle);
void		player_move(t_app *app, t_movement movement, double speed);

#endif
