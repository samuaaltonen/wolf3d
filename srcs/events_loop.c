/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_loop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 15:41:22 by saaltone          #+#    #+#             */
/*   Updated: 2022/07/15 15:48:58 by saaltone         ###   ########.fr       */
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

/*
 * Handles continuous events.
*/
int	events_loop(t_app *app)
{
	progress_animations(app);
	//mlx_hook(app->win, ON_KEYDOWN, 0, events_keydown, app);
	app_render(app);
	return (0);
}
