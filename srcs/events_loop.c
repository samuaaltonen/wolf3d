/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_loop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 15:41:22 by saaltone          #+#    #+#             */
/*   Updated: 2022/07/18 14:30:28 by saaltone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void	progress_animations(t_app *app)
{
	int	i;

	i = 0;
	while (i < MAP_MAX_OBJECT_IDS && app->object_sprites[i].image)
	{
		app->object_sprites[i].animation_step++;
		if (app->object_sprites[i].animation_step >= app->object_sprites[i].total_steps)
			app->object_sprites[i].animation_step = 0;
		i++;
	}
}

static void	handle_movement(t_app *app)
{
	if (app->conf->keystates & FORWARD_DOWN || app->conf->keystates & FORWARD_W_DOWN)
		player_move(app, FORWARD, MOVEMENT);
	if (app->conf->keystates & BACKWARD_DOWN || app->conf->keystates & BACKWARD_S_DOWN)
		player_move(app, BACKWARD, MOVEMENT);
	if (app->conf->keystates & LEFT_DOWN)
		player_move(app, LEFT, MOVEMENT);
	if (app->conf->keystates & RIGHT_DOWN)
		player_move(app, RIGHT, MOVEMENT);
	if (app->conf->keystates & ROTATE_LEFT_DOWN)
		player_rotate(app, -ROTATION);
	if (app->conf->keystates & ROTATE_RIGHT_DOWN)
		player_rotate(app, ROTATION);
}

/*
 * Handles continuous events.
*/
int	events_loop(t_app *app)
{
	progress_animations(app);
	handle_movement(app);
	app_render(app);
	return (0);
}
