/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 15:21:33 by saaltone          #+#    #+#             */
/*   Updated: 2022/07/25 13:21:18 by saaltone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
 * Checks for collisions for movement.
*/
static int	is_collision(t_app *app, t_vector2 pos)
{
	if (pos.x >= app->map_size.x - 1 || pos.y >= app->map_size.y - 1)
		return (1);
	if (pos.x < 0 || pos.y < 0)
		return (1);
	if (app->map[(int)pos.y][(int)pos.x][0] > 'A'
		&& app->map[(int)pos.y][(int)pos.x][0] != DOOR_MAP_IDENTIFIER_MOVING)
		return (1);
	return (0);
}

/**
 * Rotates player direction by given angle.
*/
void	player_rotate(t_app *app, double angle)
{
	double	temp;

	temp = app->player.direction.x;
	app->player.direction.x = app->player.direction.x * cos(angle) - app->player.direction.y * sin(angle);
	app->player.direction.y = temp * sin(angle) + app->player.direction.y * cos(angle);
	temp = app->player.camera_plane.x;
	app->player.camera_plane.x = app->player.camera_plane.x * cos(angle) - app->player.camera_plane.y * sin(angle);
	app->player.camera_plane.y = temp * sin(angle) + app->player.camera_plane.y * cos(angle);
}

/**
 * Moves player to given direction if there is no collision.
*/
void	player_move(t_app *app, t_movement movement, double speed)
{
	t_vector2	perpendicular;
	t_vector2	new_pos;

	if (movement == FORWARD)
		new_pos = (t_vector2){app->player.position.x + app->player.direction.x * speed,
				app->player.position.y + app->player.direction.y * speed};
	if (movement == BACKWARD)
		new_pos = (t_vector2){app->player.position.x - app->player.direction.x * speed, 
			app->player.position.y - app->player.direction.y * speed};
	if (movement == LEFT || movement == RIGHT)
	{
		perpendicular = ft_vector_perpendicular(app->player.direction);
		if (movement == LEFT)
			new_pos = (t_vector2){app->player.position.x - perpendicular.x * speed, 
				app->player.position.y - perpendicular.y * speed};
		if (movement == RIGHT)
			new_pos = (t_vector2){app->player.position.x + perpendicular.x * speed, 
				app->player.position.y + perpendicular.y * speed};
	}
	if (!is_collision(app, (t_vector2){new_pos.x, app->player.position.y}))
		app->player.position.x = new_pos.x;
	if (!is_collision(app, (t_vector2){app->player.position.x, new_pos.y}))
		app->player.position.y = new_pos.y;
	coin_grab(app);
}
