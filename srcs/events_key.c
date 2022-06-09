/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_key.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 14:15:51 by saaltone          #+#    #+#             */
/*   Updated: 2022/06/09 13:10:44 by saaltone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
 * Handles events for key presses (keyup).
*/
int	events_keyup(int keycode, t_app *app)
{
	if (keycode == KEY_ESC)
		exit(EXIT_SUCCESS);
	if (keycode == KEY_H)
		app->conf->toggle_help = ft_toggle(app->conf->toggle_help);
	return (0);
}

/*
 * Handles events for key presses (keydown).
*/
int	events_keydown(int keycode, t_app *app)
{
	double	temp;

	if (keycode == KEY_ARROW_RIGHT)
	{
		temp = app->player.direction.x;
		app->player.direction.x = app->player.direction.x * cos(ROTATION) - app->player.direction.y * sin(ROTATION);
		app->player.direction.y = temp * sin(ROTATION) + app->player.direction.y * cos(ROTATION);
		temp = app->player.camera_plane.x;
		app->player.camera_plane.x = app->player.camera_plane.x * cos(ROTATION) - app->player.camera_plane.y * sin(ROTATION);
		app->player.camera_plane.y = temp * sin(ROTATION) + app->player.camera_plane.y * cos(ROTATION);
	}
	if (keycode == KEY_ARROW_LEFT)
	{
		temp = app->player.direction.x;
		app->player.direction.x = app->player.direction.x * cos(-ROTATION) - app->player.direction.y * sin(-ROTATION);
		app->player.direction.y = temp * sin(-ROTATION) + app->player.direction.y * cos(-ROTATION);
		temp = app->player.camera_plane.x;
		app->player.camera_plane.x = app->player.camera_plane.x * cos(-ROTATION) - app->player.camera_plane.y * sin(-ROTATION);
		app->player.camera_plane.y = temp * sin(-ROTATION) + app->player.camera_plane.y * cos(-ROTATION);
	}
	app_render(app);
	return (0);
}