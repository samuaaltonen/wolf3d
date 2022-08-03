/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finish.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htahvana <htahvana@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/26 11:58:54 by saaltone          #+#    #+#             */
/*   Updated: 2022/08/03 16:26:22 by htahvana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
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
