/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 15:21:33 by saaltone          #+#    #+#             */
/*   Updated: 2022/07/15 15:24:29 by saaltone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

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
