/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_views.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/26 11:58:54 by saaltone          #+#    #+#             */
/*   Updated: 2022/08/08 17:59:13 by saaltone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/**
 * Covers the screen with congratulations for a moment after finding all coins.
 */
void	finish_display(t_app *app)
{
	int	r;
	int	g;
	int	b;
	int	color_wave_step;

	app->conf->finish_scene_begin += app->conf->delta_time;
	if (app->conf->finish_scene_begin >= FINISH_SCENE_DURATION)
		app->conf->coin_max++;
	flush_image(app->image);
	color_wave_step = (int)(app->conf->finish_scene_begin * 1000) % 255;
	r = (sin(0.0246f * color_wave_step + 2) + 1) * 127;
	g = (sin(0.0246f * color_wave_step) + 1) * 127;
	b = (sin(0.0246f * color_wave_step + 4) + 1) * 127;
	mlx_put_image_to_window(app->mlx, app->win, app->image->img, 0, 0);
	mlx_string_put(app->mlx, app->win, WIN_W / 2 - 120, WIN_H / 2 - 10,
		ft_rgbtoint(r, g, b), MSG_FINISH);
}

/**
 * Displays help menu.
 */
void	help_display(t_app *app)
{
	int					i;
	static const char	*h[] = {
		"[arrow left]       Rotate left", "[arrow right]      Rotate right",
		"[arrow up]/[w]     Move forward", "[arrow down]/[s]   Move backward",
		"[a]                Move left", "[d]                Move right",
		"[b]                Toggle bloom",
		"[u]                Decrease FOV", "[i]                Increase FOV",
		"[f]                Open Doors", "[esc]              Exit", NULL,
	};

	mlx_string_put(app->mlx, app->win, WIN_W / 2 - 360,
		WIN_H / 2 - 230, 0xFFFFFF, "Controls:");
	i = -1;
	while (h[++i])
		mlx_string_put(app->mlx, app->win, WIN_W / 2 - 230,
			WIN_H / 2 - 230 + i * 30, 0xFF55FF, (char *) h[i]);
}

/**
 * Updates the info string with given value backwards from given index
 */
static void	update_value(t_app *app, int value, int char_index)
{
	int	i;

	i = -1;
	while (++i < 3)
	{
		app->fps_info[char_index - i] = value % 10 + '0';
		value = value / 10;
	}
}

/**
 * Updates all the values.
 */
void	update_info(t_app *app)
{
	update_value(app, app->conf->coin_points, 18);
	update_value(app, app->conf->coin_max, 22);
	update_value(app, app->conf->fps, 30);
}
