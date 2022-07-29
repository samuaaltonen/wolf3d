/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_mouse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 15:40:40 by saaltone          #+#    #+#             */
/*   Updated: 2022/07/29 15:27:33 by saaltone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
 * Handles events for mouse key down.
*/
int	events_mouse_down(int mousecode, int x, int y, t_app *app)
{
	(void) mousecode;
	(void) x;
	(void) y;
	(void) app;
	return (0);
}

/*
 * Handles events for mouse key up.
*/
int	events_mouse_up(int mousecode, int x, int y, t_app *app)
{
	(void) mousecode;
	(void) x;
	(void) y;
	(void) app;
	return (0);
}

/*
 * Tracks mouse position and changes player direction accordingly.
*/
int	events_mouse_track(int x, int y, t_app *app)
{
	t_point	delta;

	if (app->conf->mouse_position.x != 0 && app->conf->mouse_position.y != 0)
	{
		delta.x = x - app->conf->mouse_position.x;
		if (delta.x != 0)
			player_rotate(app, (double) delta.x / MOUSE_SENSITIVITY * app->conf->delta_time);
	}
	app->conf->mouse_position = (t_point){x, y};
	return (0);
}
