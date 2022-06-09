/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 15:14:08 by saaltone          #+#    #+#             */
/*   Updated: 2022/06/08 18:30:22 by saaltone         ###   ########.fr       */
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
		app->conf->fps_time = time(NULL);
		app->conf->fps_count = 0;
	}
}

static void	help_display(t_app *app)
{
	int					i;
	static const char	*h[] = {
		"[q]     Decrease thread count", "[w]     Increase thread count",
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
	*app = (t_app *)malloc(sizeof(t_app));
	if (!(*app))
		return (0);
	return (1);
}

void	app_run(t_app *app)
{
	app->mlx = mlx_init();
	app->win = mlx_new_window(app->mlx, app->conf->win_w,
			app->conf->win_h, app->conf->win_name);
	if (!app->win)
		exit_error(MSG_ERROR_WINDOW);
	mlx_key_hook(app->win, events_keyup, app);
	mlx_hook(app->win, 2, 0, events_keydown, app);
	mlx_hook(app->win, 6, 0, events_mouse_track, app);
	mlx_mouse_hook(app->win, events_mouse, app);
	mlx_loop_hook(app->mlx, events_loop, app);
	app->image = init_image(app->mlx, app->conf);
	app->player = (t_player){
		(t_vector2d){POSITION_START_X, POSITION_START_Y},
		(t_vector2d){DIRECTION_START_X, DIRECTION_START_Y},
		(t_vector2d){CAMERA_PLANE_X, CAMERA_PLANE_Y},
	};
	if (!app->image)
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
	render_view(app);
	mlx_string_put(app->mlx, app->win, 0, 0, 0xFFFFFF, "[h] Options");
	temp = ft_itoa(app->conf->fps);
	mlx_string_put(app->mlx, app->win, 0, 20, 0xFFFFFF, "FPS:");
	mlx_string_put(app->mlx, app->win, 120, 20, 0xFFFFFF, temp);
	free(temp);
}