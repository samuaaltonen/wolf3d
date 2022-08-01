/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finish.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/26 11:58:54 by saaltone          #+#    #+#             */
/*   Updated: 2022/08/01 16:47:35 by saaltone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	finish_display(t_app *app)
{
	int				r;
	int				g;
	int				b;

	app->conf->finish_scene_begin += app->conf->delta_time;
	if (app->conf->finish_scene_begin >= FINISH_SCENE_DURATION)
		app->conf->coin_max++;
	flush_image(app->image);
	r = (sin(0.0246f * ((int)(app->conf->finish_scene_begin * 1000) % 255) + 2) + 1) * 127;
	g = (sin(0.0246f * ((int)(app->conf->finish_scene_begin * 1000) % 255)) + 1) * 127;
	b = (sin(0.0246f * ((int)(app->conf->finish_scene_begin * 1000) % 255) + 4) + 1) * 127;
	mlx_put_image_to_window(app->mlx, app->win, app->image->img, 0, 0);
	mlx_string_put(app->mlx, app->win, WIN_W / 2 - 120, WIN_H / 2 - 10,
		ft_rgbtoint(r, g, b), MSG_FINISH);
}
