/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_render.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htahvana <htahvana@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 13:14:55 by saaltone          #+#    #+#             */
/*   Updated: 2022/08/04 15:21:14 by htahvana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/**
 * Calculates distance to next side depending on ray direction.
*/
static t_vector2	get_side_distance(t_app *app, t_vector2 *pos,
	t_vector2 *ray, t_vector2 *delta_dist)
{
	t_vector2	side_dist;

	if (ray->x < 0)
		side_dist.x = (app->player.pos.x - floor(pos->x)) * delta_dist->x;
	else
		side_dist.x = (floor(pos->x) + 1.f - app->player.pos.x) * delta_dist->x;
	if (ray->y < 0)
		side_dist.y = (app->player.pos.y - floor(pos->y)) * delta_dist->y;
	else
		side_dist.y = (floor(pos->y) + 1.f - app->player.pos.y) * delta_dist->y;
	return (side_dist);
}

/**
 * Checks if position hits any wall within map or goes outside of map.
*/
static t_bool	is_wallhit(t_app *app, t_vector2 *pos, double *side)
{
	if (pos->x >= app->map_size.x
		|| pos->y >= app->map_size.y
		|| pos->x < 0
		|| pos->y < 0)
	{
		*side = 0.f;
		return (TRUE);
	}
	if (app->map[(int) pos->y][(int) pos->x][0] > 'A'
		&& (app->conf->render_moving_doors
			|| app->map[(int) pos->y][(int) pos->x][0] != DOOR_MAP_ID_MOVING))
	{
		*side = 1.f;
		return (TRUE);
	}
	return (FALSE);
}

/**
 * Calculates wall distance using DDA method (Digital differential analyzer).
 * Returns a vector containing distance and also side of wall that was hit
 * (negative for x side, positive for y side, 0 for transparent).
*/
static t_vector2	ray_dda(t_app *app, t_vector2 *pos,
	t_vector2 *ray, t_vector2 *delta_dist)
{
	t_vector2	side_dist;
	double		side;

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
			if (is_wallhit(app, pos, &side))
				return ((t_vector2){side_dist.x - delta_dist->x, -side});
			continue ;
		}
		side_dist.y += delta_dist->y;
		if (ray->y < 0)
			pos->y += -1.f;
		else
			pos->y += 1.f;
		if (is_wallhit(app, pos, &side))
			return ((t_vector2){side_dist.y - delta_dist->y, side});
	}
}

/**
 * Calculate which part of the wall was hit for textureX
 */
static int	get_texture_hit_x(t_vector2 *pos, t_vector2 *dda, t_vector2 *ray)
{
	double	wallx;
	int		tex_x;

	if (dda->y < 0)
		wallx = pos->y + dda->x * ray->y;
	else
		wallx = pos->x + dda->x * ray->x;
	wallx -= floor(wallx);
	tex_x = ((int)(wallx * (double) TEX_SIZE) & (TEX_SIZE - 1));
	if ((dda->y < 0 && ray->x > 0) || (dda->y > 0 && ray->y < 0))
		tex_x = TEX_SIZE - tex_x - 1;
	return (tex_x);
}

/**
 * Casts a ray with given x coordinate (window coordinate). Returns 1 on wall
 * hit.
*/
int	raycast(t_app *app, int x, t_rayhit *rayhit)
{
	double		camera_x;
	t_vector2	ray;
	t_vector2	pos;
	t_vector2	delta_dist;
	t_vector2	dda;

	camera_x = 2 * x / (double) WIN_W - 1.f;
	ray.x = app->player.dir.x + app->player.cam.x * camera_x;
	ray.y = app->player.dir.y + app->player.cam.y * camera_x;
	pos = (t_vector2){app->player.pos.x, app->player.pos.y};
	delta_dist = (t_vector2){fabs(1.f / ray.x), fabs(1.f / ray.y)};
	dda = ray_dda(app, &pos, &ray, &delta_dist);
	if (dda.y == 0.f || dda.y == -0.f)
		*rayhit = (t_rayhit){0, EMPTY_MAP_ID, 0, dda.x, pos};
	else
		*rayhit = (t_rayhit){
			get_cardinal(app, &pos, dda.y),
			app->map[(int) pos.y][(int) pos.x][get_cardinal(app, &pos,
				dda.y)] - 1,
			get_texture_hit_x(&pos, &dda, &ray),
			dda.x,
			pos
		};
	return (1);
}
