/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_key.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 14:15:51 by saaltone          #+#    #+#             */
/*   Updated: 2022/07/08 12:38:54 by saaltone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
 * Checks for collisions for movement.
*/
static int	is_collision(t_app *app, t_vector2 pos)
{
	if (pos.x >= MAP_WIDTH - 1 || pos.y >= MAP_HEIGHT - 1)
		return (1);
	if (pos.x < 0 || pos.y < 0)
		return (1);
	if (app->map[(int)pos.y][(int)pos.x])
		return (1);
	return (0);
}

static void	handle_player_direction(int keycode, t_app *app)
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
	ft_printf("Direction vector: %20f %20f\n", app->player.direction.x, app->player.direction.y);
}

static void	handle_player_position(int keycode, t_app *app)
{
	if (keycode == KEY_ARROW_UP || keycode == KEY_W)
	{
		if (!is_collision(app, (t_vector2){
			app->player.position.x + app->player.direction.x * MOVEMENT, 
			app->player.position.y + app->player.direction.y * MOVEMENT}))
			app->player.position = (t_vector2){
				app->player.position.x + app->player.direction.x * MOVEMENT,
				app->player.position.y + app->player.direction.y * MOVEMENT
			};
	}
	if (keycode == KEY_ARROW_DOWN || keycode == KEY_S)
	{
		if (!is_collision(app, (t_vector2){
			app->player.position.x - app->player.direction.x * MOVEMENT, 
			app->player.position.y - app->player.direction.y * MOVEMENT}))
			app->player.position = (t_vector2){
				app->player.position.x - app->player.direction.x * MOVEMENT,
				app->player.position.y - app->player.direction.y * MOVEMENT
			};
	}
}

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
	handle_player_direction(keycode, app);
	handle_player_position(keycode, app);
	app_render(app);
	return (0);
}
