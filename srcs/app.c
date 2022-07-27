/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htahvana <htahvana@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 15:14:08 by saaltone          #+#    #+#             */
/*   Updated: 2022/07/27 15:17:43 by htahvana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
 * Calculates FPS and scales movement speeds according to frame time. 
*/
static void	update_fps_counter(t_app *app)
{
	double	time_since;
	clock_t	time_now;

	app->conf->fps_count++;
	time_now = clock();
	time_since = (time_now - app->conf->fps_clock) / (double) CLOCKS_PER_SEC;
	if (time_since > FPS_UPDATE_FREQUENCY)
	{
		app->conf->fps = (int) (app->conf->fps_count / time_since);
		app->conf->delta_time = time_since / (double) app->conf->fps_count;
		app->conf->fps_clock = time_now;
		app->conf->fps_count = 1;
		app->conf->movement_speed = MOVEMENT_SPEED * app->conf->delta_time ;
		app->conf->rotation_speed = ROTATION_SPEED * app->conf->delta_time ;
	}
}

/**
 * Displays help manu.
*/
static void	help_display(t_app *app)
{
	int					i;
	static const char	*h[] = {
		"[arrow left]       Rotate left" , "[arrow right]      Rotate right",
		"[arrow up]/[w]     Move forward", "[arrow down]/[s]   Move backward",
		"[a]                Move left",    "[d]                Move right",
		"[u]                Decrease FOV", "[i]                Increase FOV",
		"[f]                Open Doors",   "[esc]              Exit", NULL,
	};

	flush_image(app->image);
	mlx_clear_window(app->mlx, app->win);
	mlx_string_put(app->mlx, app->win, app->conf->win_w / 2 - 360,
		app->conf->win_h / 2 - 230, 0xFFFFFF, "Controls:");
	i = -1;
	while (h[++i])
		mlx_string_put(app->mlx, app->win, app->conf->win_w / 2 - 230,
			app->conf->win_h / 2 - 230 + i * 30, 0xFF55FF, (char *) h[i]);
}

int	app_init(t_app **app)
{
	int	x;
	int	y;

	*app = (t_app *)malloc(sizeof(t_app));
	if (!(*app) || !check_map(*app))
		return (0);
	(*app)->map = (char ***)malloc(sizeof(char *) * (*app)->map_size.y);
	if (!((*app)->map))
		return (0);
	y = -1;
	while (++y < (*app)->map_size.y)
	{
		x = -1;
		(*app)->map[y] = (char **)malloc(sizeof(char *) * (*app)->map_size.x);
		if (!((*app)->map[y]))
			return (0);
		while (++x < (*app)->map_size.x)
		{
			(*app)->map[y][x] = (char *)malloc(sizeof(char) * MAP_BYTES);
			if (!((*app)->map[y][x]))
				return (0);
		}
	}
	return (1);
}

void	app_run(t_app *app)
{
	app->mlx = mlx_init();
	app->win = mlx_new_window(app->mlx, app->conf->win_w,
			app->conf->win_h, app->conf->win_name);
	//app->fps_info = "Coins collected:    FPS:   ";
	ft_strcpy(app->fps_info, "Coins collected:   /    FPS:   ");
	mlx_do_key_autorepeatoff(app->mlx);
	if (!app->win)
		exit_error(MSG_ERROR_WINDOW);
	mlx_hook(app->win, ON_KEYUP, KEY_RELEASE_MASK, events_keyup, app);
	mlx_hook(app->win, ON_KEYDOWN, KEY_PRESS_MASK, events_keydown, app);
	mlx_hook(app->win, ON_MOUSEMOVE, POINTER_MOTION_MASK, events_mouse_track, app);
	mlx_hook(app->win, ON_MOUSEDOWN, BUTTON_PRESS_MASK, events_mouse_down, app);
	mlx_hook(app->win, ON_MOUSEUP, BUTTON_RELEASE_MASK, events_mouse_up, app);
	mlx_hook(app->win, ON_DESTROY, NO_EVENT_MASK, events_window_destroy, app);
	mlx_loop_hook(app->mlx, events_loop, app);
	app->image = init_image(app->mlx, WIN_W, WIN_H);
	app->depthmap = init_image(app->mlx, WIN_W, WIN_H);
	app->sprite = init_xpm_image(app->mlx,
		TEX_SIZE * TEX_COUNT, TEX_SIZE, TEXTURE_PANELS);
	app->player = (t_player){
		(t_vector2){POSITION_START_X, POSITION_START_Y},
		(t_vector2){DIRECTION_START_X, DIRECTION_START_Y},
		(t_vector2){0, 0}};
	load_object_textures(app);
	init_camera_plane(app);
	if (!app->image || !app->depthmap || !app->sprite)
		exit_error(NULL);
	app_render(app);
	mlx_loop(app->mlx);
}

// Updates the info string with given value backwards from given index
static void update_info(t_app *app, int value, int char_index)
{
	int	i;

	i = -1;
	while (++i < 3)
	{
		app->fps_info[char_index - i] = value % 10 + '0';
		value = value / 10;
	}
}

void	app_render(t_app *app)
{
	if (app->conf->toggle_help)
		return (help_display(app));
	if (app->conf->coin_points >= app->conf->coin_max)
		return (finish_display(app));
	update_fps_counter(app);
	flush_image(app->image);
	flush_image(app->depthmap);
	mlx_put_image_to_window(app->mlx, app->win, app->image->img, 0, 0);
	render_multithreading(app, render_background);
	render_multithreading(app, render_view);
	render_multithreading(app, render_objects);
	if (app->conf->has_moving_doors)
		render_moving_doors(app);
	mlx_put_image_to_window(app->mlx, app->win, app->image->img, 0, 0);
	if (DEPTH)
	{
		read_depthmap(app->depthmap);
		mlx_put_image_to_window(app->mlx, app->win, app->depthmap->img, 0, 0);
	}
	mlx_string_put(app->mlx, app->win, 0, 0, 0xFFFFFF, "[h] Options");
	update_info(app, app->conf->coin_points, 18);
	update_info(app, app->conf->coin_max, 22);
	update_info(app, app->conf->fps, 30);
	mlx_string_put(app->mlx, app->win, 0, 20, 0xFFFFFF, app->fps_info);
}
