/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htahvana <htahvana@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 15:14:08 by saaltone          #+#    #+#             */
/*   Updated: 2022/08/03 16:35:56 by htahvana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
 * Calculates FPS and scales movement speeds according to frame time. 
*/
static void	update_fps_counter(t_app *app)
{
	struct timespec	time_now;
	struct timespec	time_since;

	clock_gettime(CLOCK_REALTIME, &time_now);
	time_since.tv_nsec = time_now.tv_nsec - app->conf->fps_clock.tv_nsec;
	time_since.tv_sec = time_now.tv_sec - app->conf->fps_clock.tv_sec;
	app->conf->delta_time = (double)time_since.tv_sec
		+ 1.0e-9 * time_since.tv_nsec;
	app->conf->fps = (int)(1 / app->conf->delta_time);
	app->conf->fps_clock = time_now;
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
	app->win = mlx_new_window(app->mlx, WIN_W, WIN_H, WIN_NAME);
	ft_strcpy(app->fps_info, "Coins collected:   /    FPS:   ");
	mlx_do_key_autorepeatoff(app->mlx);
	if (!app->win)
		exit_error(MSG_ERROR_WINDOW);
	mlx_hook(app->win, ON_KEYUP, KEY_RELEASE_MASK, events_keyup, app);
	mlx_hook(app->win, ON_KEYDOWN, KEY_PRESS_MASK, events_keydown, app);
	mlx_hook(app->win, ON_MOUSEMOVE, 64, events_mouse_track, app);
	mlx_hook(app->win, ON_DESTROY, NO_EVENT_MASK, events_window_destroy, app);
	mlx_loop_hook(app->mlx, events_loop, app);
	app->image = init_image(app->mlx, WIN_W, WIN_H);
	app->depthmap = init_image(app->mlx, WIN_W, WIN_H);
	app->sprite = init_xpm_image(app->mlx,
			TEX_SIZE * TEX_COUNT, TEX_SIZE, TEXTURE_PANELS);
	app->bg = init_xpm_image(app->mlx, 512, 128, "./assets/bg.xpm");
	app->player = (t_player){
		(t_vector2){POSITION_START_X, POSITION_START_Y},
		(t_vector2){DIRECTION_START_X, DIRECTION_START_Y},
		(t_vector2){0, 0}};
	load_object_textures(app);
	init_camera_plane(app);
	if (!app->image || !app->depthmap || !app->sprite || !check_textures(app))
		exit_error(MSG_ERROR_TEXTURE_LOAD_FAILED);
}

void	app_render(t_app *app)
{
	if (app->conf->toggle_help)
		return (help_display(app));
	if (app->conf->coin_max > 0
		&& app->conf->coin_points >= app->conf->coin_max)
		return (finish_display(app));
	update_fps_counter(app);
	flush_image(app->image);
	flush_image(app->depthmap);
	mlx_put_image_to_window(app->mlx, app->win, app->image->img, 0, 0);
	render_multithreading(app, render_skybox);
	render_multithreading(app, render_background);
	render_multithreading(app, render_view);
	render_multithreading(app, render_objects);
	if (app->conf->has_moving_doors)
		render_moving_doors(app);
	mlx_put_image_to_window(app->mlx, app->win, app->image->img, 0, 0);
	if (app->conf->toggle_bloom)
	{
		render_multithreading(app, render_bloom);
		render_multithreading(app, read_bloom);
	}
	mlx_put_image_to_window(app->mlx, app->win, app->depthmap->img, 0, 0);
	mlx_string_put(app->mlx, app->win, 0, 0, 0xFFFFFF, "[h] Options");
	update_info(app);
	mlx_string_put(app->mlx, app->win, 0, 20, 0xFFFFFF, app->fps_info);
}
