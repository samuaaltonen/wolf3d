/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 14:20:36 by saaltone          #+#    #+#             */
/*   Updated: 2022/08/04 14:36:43 by saaltone         ###   ########.fr       */
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
# define MSG_ERROR_TEXTURE_LOAD_FAILED "Texture files are invalid."
# define MSG_ERROR_MAP_INVALID "Map file is invalid."
# define MSG_ERROR_MAP_SIZE "Map size is too small."
# define MSG_FINISH "Congratulations, you won!"
# define THREAD_COUNT 8
# define MAP_FILE "./map.txt"
# define MAP_BYTES 4
# define IMAGE_PIXEL_BYTES 4
# define MAP_MAX_OBJECT_IDS 4
# define COLLISION_OFFSET 0.25f
# define DIRECTION_START_X -1.f
# define DIRECTION_START_Y 0.f
# define FOV 66
# define DEG_IN_RADIAN 0.01745f
# define RADIAN_IN_DEG 57.29578f
# define ROTATION_SPEED 1.8f
# define MOVEMENT_SPEED 3.2f
# define MAX_RAY_DISTANCE 20.f
# define BLOOM_SIZE 15
# define TEX_SIZE 64
# define TEX_COUNT 20
# define MOUSE_SENSITIVITY 10.f
# define FPS_UPDATE_FREQUENCY 0.0625f
# define COIN_SPIN_MAP_IDENTIFIER 'B'
# define COIN_WHIRL_MAP_IDENTIFIER 'C'
# define COIN_GRAB_DISTANCE_THRESHOLD 0.5f
# define EMPTY_MAP_ID 'X'
# define DOOR_MAP_ID 'T'
# define DOOR_MAP_ID_MOVING 'Z'
# define DOOR_ANIMATION_DURATION 0.125f
# define DOOR_CLOSING_THRESHOLD 5.f
# define DOOR_ACTION_DISTANCE_THRESHOLD 2.f
# define FINISH_SCENE_DURATION 2.f
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

/**
 * MLX event enumeration
*/
enum e_mlx_events {
	ON_KEYDOWN = 2,
	ON_KEYUP = 3,
	ON_MOUSEMOVE = 6,
	ON_DESTROY = 17
};

/**
 * MLX mask enumeration
*/
enum e_mlx_event_masks {
	NO_EVENT_MASK = 0,
	KEY_PRESS_MASK = 1,
	KEY_RELEASE_MASK = 2,
	BUTTON_PRESS_MASK = 4,
	BUTTON_RELEASE_MASK = 8,
	MOTION_MASK = 64
};

/**
 * Unsigned char
*/
typedef unsigned char	t_uint8;

/**
 * Cardinal direction enumeration.
*/
typedef enum e_cardinal {
	NORTH = 0,
	EAST = 1,
	SOUTH = 2,
	WEST = 3
}	t_cardinal;

/**
 * Movement direction enumeration.
*/
typedef enum e_movement {
	FORWARD = 0,
	BACKWARD = 1,
	LEFT = 2,
	RIGHT = 3
}	t_movement;

/**
 * Keystate enumeration.
 * Keystate enumerations use powers of 2 for bit stacking operations.
*/
enum e_keystate {
	FORWARD_DOWN = 1,
	FORWARD_W_DOWN = 2,
	BACKWARD_DOWN = 4,
	BACKWARD_S_DOWN = 8,
	LEFT_DOWN = 16,
	RIGHT_DOWN = 32,
	ROTATE_LEFT_DOWN = 64,
	ROTATE_RIGHT_DOWN = 128
};

/**
 * Door state enumeration.
*/
typedef enum e_doorstate {
	CLOSED = 0,
	OPEN = 1,
	CLOSING = 2,
	OPENING = 3
}	t_doorstate;

/**
 * Struct for integer coordinate point.
*/
typedef struct s_point
{
	int				x;
	int				y;
}	t_point;

/**
 * Struct for images.
*/
typedef struct s_image
{
	int				bpp;
	int				line_size;
	int				width;
	int				height;
	int				endian;
	char			*img;
	char			*data;
}	t_image;

/**
 * Struct for configuration variables of the application.
*/
typedef struct s_conf
{
	int				toggle_help;
	int				toggle_bloom;
	int				fps;
	struct timespec	fps_clock;
	int				fps_count;
	int				coin_points;
	int				coin_max;
	double			delta_time;
	double			skybox_offset;
	int				fov;
	t_point			mouse_position;
	int				keystates;
	double			movement_speed;
	double			rotation_speed;
	int				has_moving_doors;
	int				render_moving_doors;
	double			finish_scene_begin;
}	t_conf;

/**
 * Struct for threads about their specific drawing areas.
*/
typedef struct s_thread_data
{
	void			*app;
	int				x_start;
	int				x_end;
	int				y_start;
	int				y_end;
	int				id;
}	t_thread_data;

/**
 * Struct for sprite data.
 * 
 * char			*path;
 * t_image		*image;
 * double		offset_multiplier;
 * int			animation_step;
 * int			total_steps;
 * int			mirrored;
*/
typedef struct s_sprite_data
{
	char			*path;
	t_image			*image;
	double			offset_multiplier;
	int				animation_step;
	int				total_steps;
	int				mirrored;
}	t_sprite_data;

/**
 * Struct for player. Contains players position, direction and camera plane.
*/
typedef struct s_player
{
	t_vector2		pos;
	t_vector2		dir;
	t_vector2		cam;
}	t_player;

/**
 * Struct for objects.
 * 
 * t_vector2	position;
 * int			sprite_id;
 * int			width;
 * int			height;
 * double		distance;
 * t_vector2	texture_offset;
 * int			frame_id;
 * int			active;
*/
typedef struct s_object
{
	t_vector2		position;
	int				sprite_id;
	int				width;
	int				height;
	double			distance;
	t_vector2		texture_offset;
	int				frame_id;
	int				active;
}	t_object;

/**
 * Struct for doors. Contains information about doors position, state
 * and animation.
 * 
 * t_vector2		position;
 * t_doorstate		state;
 * double			animation_begin;
*/
typedef struct s_door
{
	t_vector2		position;
	t_doorstate		state;
	double			animation_begin;
}	t_door;

/**
 * Struct for rayhit. Contains information about cardinal direction of the wall
 * that was hit, its type, x coordinate of the texture, distance of the camera
 * plane and position of the hit.
 * 
 * t_cardinal	direction;
 * char			type;
 * int			tex_x;
 * double		distance;
 * t_vector2	hit_pos;
*/
typedef struct s_rayhit
{
	t_cardinal		direction;
	char			type;
	int				tex_x;
	double			distance;
	t_vector2		hit_pos;
}	t_rayhit;

/**
 * Struct for the application.
*/
typedef struct s_app
{
	t_conf			*conf;
	t_image			*image;
	t_image			*depthmap;
	void			*mlx;
	void			*win;
	t_thread_data	thread_info[THREAD_COUNT];
	t_point			map_size;
	char			***map;
	t_player		player;
	t_image			*sprite;
	t_image			*bg;
	t_object		*objects;
	t_door			*doors;
	int				door_count;
	t_sprite_data	object_sprites[MAP_MAX_OBJECT_IDS];
	int				object_count;
	char			fps_info[31];
}	t_app;

/**
 * Messages
*/
void		exit_error(char *message);

/**
 * Configuration
*/
int			conf_init(t_app *app);
void		init_thread_info(t_app *app);
void		init_camera_plane(t_app *app);

/**
 * Application
*/
int			app_init(t_app **app, char *path);
void		app_run(t_app *app);
void		app_render(t_app *app);
void		update_info(t_app *app);
void		help_display(t_app *app);

/**
 * Images
*/
t_image		*init_image(void *mlx, int x, int y);
void		put_pixel_to_image(t_image *image, int x, int y, int color);
void		put_pixel_to_image_depth(t_app *app, t_point point, int color,
				float distance);
void		put_pixel_to_image_test(t_app *app, t_point point, int color,
				float distance);
void		flush_image(t_image *image);
int			get_pixel_color(t_image *image, int x, int y);
t_image		*init_xpm_image(void *mlx, int width, int height, char *path);

/**
 * Events
*/
int			events_keyup(int keycode, t_app *app);
int			events_keydown(int keycode, t_app *app);
int			events_mouse_down(int mousecode, int x, int y, t_app *app);
int			events_mouse_up(int mousecode, int x, int y, t_app *app);
int			events_mouse_track(int x, int y, t_app *app);
int			events_window_destroy(void);
int			events_loop(t_app *app);

/**
 * Graphics
*/
void		*render_view(void *data);
void		*render_background(void *data);
void		*render_objects(void *data);
void		render_multithreading(t_app *app, void *(*renderer)(void *));
void		*render_skybox(void *data);
void		*render_bloom(void *data);
void		*read_bloom(void *data);

/**
 * Checker
*/
int			parse_map(t_app *app, char *path);
int			check_map(t_app *app, char *path);
int			check_textures(t_app *app);

/**
 * Walls
*/
int			raycast(t_app *app, int x, t_rayhit *rayhit);

/*
 * Objects
*/
void		init_objects(t_app *app);
void		load_object_textures(t_app *app);
void		cast_objects(t_app *app);
void		draw_object(t_app *app, t_object *object, int screen_x);
void		coin_grab(t_app *app);

/**
 * Player
*/
void		player_rotate(t_app *app, double angle);
void		player_move(t_app *app, t_movement movement, double speed);

/**
 * Doors
*/
void		init_doors(t_app *app);
void		door_action(t_app *app);
void		progress_doors(t_app *app);
void		render_moving_doors(t_app *app);

/**
 * Finish scene
*/
void		finish_display(t_app *app);

/**
 * Helper functions
*/
double		get_radial_direction(t_vector2 *vector);
void		clamp_distance(double *distance);
t_cardinal	get_cardinal(t_app *app, t_vector2 *pos, double side);

#endif
