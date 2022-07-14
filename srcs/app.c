/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htahvana <htahvana@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 15:14:08 by saaltone          #+#    #+#             */
/*   Updated: 2022/07/14 14:34:15 by htahvana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
 * Calculates FPS and displays it.
*/
static void	update_fps_counter(t_app *app)
{
	int	time_since;

	app->conf->fps_count++;
	if (time(NULL) > app->conf->fps_time + 2)
	{
		time_since = time(NULL) - app->conf->fps_time;
		app->conf->fps = app->conf->fps_count / time_since;
		app->conf->delta_time = time_since / (double) app->conf->fps_count;
		app->conf->fps_time = time(NULL);
		app->conf->fps_count = 0;
	}
}

static void	help_display(t_app *app)
{
	int					i;
	static const char	*h[] = {
		"[u]     Decrease FOV", "[i]     Increase FOV",
		"[esc]   Exit", NULL,
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
	int x;
	int y;

	*app = (t_app *)malloc(sizeof(t_app));
	if (!(*app) || !check_map(*app))
		return (0);
	(*app)->map = (char ***)malloc(sizeof(char *) * (*app)->map_size.y);
	if (!((*app)->map))
		return (0);
	y = 0;
	while (y < (*app)->map_size.y)
	{
		x = 0;
		(*app)->map[y] = (char **)malloc(sizeof(char *) * (*app)->map_size.x);
		if (!((*app)->map[y]))
			return (0);
		while(x < (*app)->map_size.x)
		{
			(*app)->map[y][x] = (char *)malloc(sizeof(char) * MAP_BYTES);
			if (!((*app)->map[y][x]))
				return (0);
			x++;
		}
		y++;
	}
	return (1);
}

void	app_run(t_app *app)
{
	app->mlx = mlx_init();
	app->win = mlx_new_window(app->mlx, app->conf->win_w,
			app->conf->win_h, app->conf->win_name);
	if (!app->win)
		exit_error(MSG_ERROR_WINDOW);
	mlx_hook(app->win, ON_KEYUP, 0, events_keyup, app);
	mlx_hook(app->win, ON_KEYDOWN, 0, events_keydown, app);
	mlx_hook(app->win, ON_MOUSEMOVE, 0, events_mouse_track, app);
	mlx_hook(app->win, ON_DESTROY, 0, events_window_destroy, app);
	mlx_hook(app->win, ON_MOUSEDOWN, 0, events_mouse_down, app);
	mlx_hook(app->win, ON_MOUSEUP, 0, events_mouse_up, app);
	mlx_loop_hook(app->mlx, events_loop, app);
	app->image = init_image(app->mlx, WIN_W, WIN_H);
	app->sprite = init_xpm_image(app->mlx, TEX_SIZE * 8, TEX_SIZE, "./wolftextures.xpm");
	app->coin = init_xpm_image(app->mlx, TEX_SIZE * 64, TEX_SIZE, "./coin_spritesheet.xpm");
	app->player = (t_player){
		(t_vector2){POSITION_START_X, POSITION_START_Y},
		(t_vector2){DIRECTION_START_X, DIRECTION_START_Y},
		(t_vector2){0, 0}
	};
	init_camera_plane(app);
	if (!app->image || !app->sprite || !app->coin)
		exit_error(NULL);
	app_render(app);
	mlx_loop(app->mlx);
}

void	app_render(t_app *app)
{
	char	*temp;

	if (app->conf->toggle_help)
		return (help_display(app));
	update_fps_counter(app);
	flush_image(app->image);
	render_background(app);
	render_multithreading(app);
	mlx_string_put(app->mlx, app->win, 0, 0, 0xFFFFFF, "[h] Options");
	temp = ft_itoa(app->conf->fps);
	mlx_string_put(app->mlx, app->win, 0, 20, 0xFFFFFF, "FPS:");
	mlx_string_put(app->mlx, app->win, 120, 20, 0xFFFFFF, temp);
	free(temp);
}
