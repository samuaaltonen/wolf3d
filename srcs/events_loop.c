/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_loop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 15:41:22 by saaltone          #+#    #+#             */
/*   Updated: 2022/07/14 14:52:31 by saaltone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
 * Handles continuous events.
*/
int	events_loop(t_app *app)
{
	app->conf->object_step++;
	if (app->conf->object_step >= ANIMATION_MAX_SEQUENCE)
		app->conf->object_step = 0;
	app_render(app);
	mlx_put_image_to_window(app->mlx,app->win,app->sprite->img,0,0);
	return (0);
}
