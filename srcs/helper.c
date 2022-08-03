/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 14:32:04 by saaltone          #+#    #+#             */
/*   Updated: 2022/08/03 14:41:03 by saaltone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/**
 * Returns cardinal direction 
*/
t_cardinal	get_cardinal(t_app *app, t_vector2 *pos, double side)
{
	if (side < 0)
	{
		if (app->player.pos.x < pos->x)
			return (EAST);
		return (WEST);
	}
	if (app->player.pos.y < pos->y)
		return (SOUTH);
	return (NORTH);
}

/**
 * Clamps distance to maximum distance.
*/
void	clamp_distance(double *distance)
{
	if (*distance > MAX_RAY_DISTANCE)
		*distance = MAX_RAY_DISTANCE;
}

/**
 * Returns radial direction of a vector.
*/
double	get_radial_direction(t_vector2 *vector)
{
	double	rad;

	rad = atan2(vector->x, vector->y);
	if (rad < 0)
		rad = rad + 2 * M_PI;
	rad = rad * (180 / M_PI);
	return (rad);
}
