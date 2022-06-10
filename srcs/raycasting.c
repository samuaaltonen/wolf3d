/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 13:14:55 by saaltone          #+#    #+#             */
/*   Updated: 2022/06/10 11:15:55 by saaltone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
 * Calculates distance to next side depending on ray direction.
*/
static t_vector2	get_side_distance(t_app *app, t_vector2 *pos,
	t_vector2 *ray, t_vector2 *delta_dist)
{
	t_vector2	side_dist;

	if (ray->x < 0)
		side_dist.x = (app->player.position.x - pos->x) * delta_dist->x;
	else
		side_dist.x = (pos->x + 1.f - app->player.position.x) * delta_dist->x;
	if (ray->y < 0)
		side_dist.y = (app->player.position.y - pos->y) * delta_dist->y;
	else
		side_dist.y = (pos->y + 1.f - app->player.position.y) * delta_dist->y;
	return (side_dist);
}

/*
 * Calculates wall distance using DDA method (Digital differential analyzer). 
*/
static double	get_wall_distance(t_app *app, t_vector2 *pos,
	t_vector2 *ray, t_vector2 *delta_dist)
{
	t_vector2	side_dist;

	side_dist = get_side_distance(app, pos, ray, delta_dist);
	while (1)
	{
		if (side_dist.x < side_dist.y)
		{
			side_dist.x += delta_dist->x;
			if (ray->x < 0)
				pos->x += -1.f;
			else
				pos->x += 1.f;
			if (app->map[(int) pos->y][(int) pos->x])
				return (side_dist.x - delta_dist->x);
			continue ;
		}
		side_dist.y += delta_dist->y;
		if (ray->y < 0)
			pos->y += -1.f;
		else
			pos->y += 1.f;
		if (app->map[(int) pos->y][(int) pos->x])
			return (side_dist.y - delta_dist->y);
	}
}

/*
 * Casts a ray with given x coordinate (window coordinate).
*/
int	raycast(t_app *app, int x, double *distance)
{
	double		camera_x;
	t_vector2	ray;
	t_vector2	pos;
	t_vector2	delta_dist;

	camera_x = 2 * x / (double) WIN_W - 1.f;
	ray.x = app->player.direction.x + app->player.camera_plane.x * camera_x;
	ray.y = app->player.direction.y + app->player.camera_plane.y * camera_x;
	pos = (t_vector2){app->player.position.x, app->player.position.y};
	delta_dist = (t_vector2){fabs(1.f / ray.x), fabs(1.f / ray.y)};
	*distance = get_wall_distance(app, &pos, &ray, &delta_dist);
	return (app->map[(int) pos.y][(int) pos.x]);
}
