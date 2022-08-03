/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_key.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 14:15:51 by saaltone          #+#    #+#             */
/*   Updated: 2022/08/03 15:19:33 by saaltone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/**
 * Handles events for key presses (keyup).
*/
int	events_keyup(int keycode, t_app *app)
{
	if (keycode == KEY_ARROW_RIGHT)
		app->conf->keystates ^= ROTATE_RIGHT_DOWN;
	if (keycode == KEY_ARROW_LEFT)
		app->conf->keystates ^= ROTATE_LEFT_DOWN;
	if (keycode == KEY_ARROW_UP)
		app->conf->keystates ^= FORWARD_DOWN;
	if (keycode == KEY_ARROW_DOWN)
		app->conf->keystates ^= BACKWARD_DOWN;
	if (keycode == KEY_W)
		app->conf->keystates ^= FORWARD_W_DOWN;
	if (keycode == KEY_S)
		app->conf->keystates ^= BACKWARD_S_DOWN;
	if (keycode == KEY_A)
		app->conf->keystates ^= LEFT_DOWN;
	if (keycode == KEY_D)
		app->conf->keystates ^= RIGHT_DOWN;
	if (keycode == KEY_F)
		door_action(app);
	if (keycode == KEY_ESC)
		exit(EXIT_SUCCESS);
	if (keycode == KEY_H)
		app->conf->toggle_help = ft_toggle(app->conf->toggle_help);
	if (keycode == KEY_B)
		app->conf->toggle_bloom = ft_toggle(app->conf->toggle_bloom);
	return (0);
}

/**
 * Handles events for key presses (keydown).
*/
int	events_keydown(int keycode, t_app *app)
{
	if (keycode == KEY_ARROW_RIGHT)
		app->conf->keystates |= ROTATE_RIGHT_DOWN;
	if (keycode == KEY_ARROW_LEFT)
		app->conf->keystates |= ROTATE_LEFT_DOWN;
	if (keycode == KEY_ARROW_UP)
		app->conf->keystates |= FORWARD_DOWN;
	if (keycode == KEY_ARROW_DOWN)
		app->conf->keystates |= BACKWARD_DOWN;
	if (keycode == KEY_W)
		app->conf->keystates |= FORWARD_W_DOWN;
	if (keycode == KEY_S)
		app->conf->keystates |= BACKWARD_S_DOWN;
	if (keycode == KEY_A)
		app->conf->keystates |= LEFT_DOWN;
	if (keycode == KEY_D)
		app->conf->keystates |= RIGHT_DOWN;
	if (keycode == KEY_U)
		app->conf->fov--;
	if (keycode == KEY_I)
		app->conf->fov++;
	if (keycode == KEY_U || keycode == KEY_I)
		init_camera_plane(app);
	app_render(app);
	return (0);
}
