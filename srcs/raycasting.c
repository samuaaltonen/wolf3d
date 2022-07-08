/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htahvana <htahvana@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 13:14:55 by saaltone          #+#    #+#             */
/*   Updated: 2022/07/08 13:32:45 by htahvana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
 * Returns cardinal direction 
*/
static t_cardinal	get_cardinal(t_app *app, t_vector2 *pos, double side)
{
	if (side < 0) {
		if (app->player.position.x < pos->x)
			return (EAST);
		return (WEST);
	}
	if (app->player.position.y < pos->y)
		return (SOUTH);
	return (NORTH);
}

/*
 * Calculates distance to next side depending on ray direction.
*/
static t_vector2	get_side_distance(t_app *app, t_vector2 *pos,
	t_vector2 *ray, t_vector2 *delta_dist)
{
	t_vector2	side_dist;

	if (ray->x < 0)
		side_dist.x = (app->player.position.x - floor(pos->x)) * delta_dist->x;
	else
		side_dist.x = (floor(pos->x) + 1.f - app->player.position.x) * delta_dist->x;
	if (ray->y < 0)
		side_dist.y = (app->player.position.y - floor(pos->y)) * delta_dist->y;
	else
		side_dist.y = (floor(pos->y) + 1.f - app->player.position.y) * delta_dist->y;
	return (side_dist);
}

/*
 * Calculates wall distance using DDA method (Digital differential analyzer).
 * Returns a vector containing distance and also side of wall that was hit
*/
static t_vector2	ray_dda(t_app *app, t_vector2 *pos,
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
				return ((t_vector2){side_dist.x - delta_dist->x, -1.0f});
			continue ;
		}
		side_dist.y += delta_dist->y;
		if (ray->y < 0)
			pos->y += -1.f;
		else
			pos->y += 1.f;
		if (app->map[(int) pos->y][(int) pos->x])
			return ((t_vector2){side_dist.y - delta_dist->y, 1.0f});
	}
}

/*
 * Calculate which part of the wall was hit for textureX
 */
int	get_texture_hit_x(t_vector2 *pos, t_vector2 dda, t_vector2 ray)
{
	double wallx;

	if(dda.y < 0)
		wallx = pos->y + dda.x * ray.y;
	else
		wallx = pos->x + dda.x * ray.x;
	wallx -= floor(wallx);

	int tex_x;
	tex_x = (int)(wallx * (double)TEX_SIZE);
	if(dda.y < 0 && ray.x > 0)
		tex_x = TEX_SIZE - tex_x - 1;
	if(dda.y > 0 && ray.y < 0)
		tex_x = TEX_SIZE - tex_x - 1;
	return (tex_x);
}

/*
 * Casts a ray with given x coordinate (window coordinate).
*/
t_rayhit	raycast(t_app *app, int x, double *distance)
{
	double		camera_x;
	t_vector2	ray;
	t_vector2	pos;
	t_vector2	delta_dist;
	t_vector2	dda;

	camera_x = 2 * x / (double) WIN_W - 1.f;
	ray.x = app->player.direction.x + app->player.camera_plane.x * camera_x;
	ray.y = app->player.direction.y + app->player.camera_plane.y * camera_x;
	pos = (t_vector2){app->player.position.x, app->player.position.y};
	delta_dist = (t_vector2){fabs(1.f / ray.x), fabs(1.f / ray.y)};
	dda = ray_dda(app, &pos, &ray, &delta_dist);
	*distance = dda.x;
	return ((t_rayhit){
		get_cardinal(app, &pos, dda.y),
		app->map[(int) pos.y][(int) pos.x],
		get_texture_hit_x(&pos, dda, ray)
	});
}
