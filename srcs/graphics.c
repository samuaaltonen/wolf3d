/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 14:32:45 by saaltone          #+#    #+#             */
/*   Updated: 2022/06/09 13:12:29 by saaltone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
 * Draws vertical line to image based on wall distance (closer = higher)
*/
static void	draw_vertical_line(t_app *app, int x, int height, int texture)
{
	int	start_pixel;
	int	end_pixel;
	int	i;

	if (height > WIN_H)
		height = WIN_H;
	start_pixel = WIN_H / 2 - height / 2;
	end_pixel = WIN_H / 2 + height / 2;
	if (start_pixel < 0)
		start_pixel = 0;
	if (end_pixel >= WIN_H)
		end_pixel = WIN_H - 1;
	i = 0;
	while (i < height)
	{
		put_pixel_to_image(app->image, x, start_pixel + i, 0xFFFFFF / texture);
		i++;
	}
}

/*
 * Calculates distance to next side depending on ray direction.
*/
static t_vector2d	get_side_distance(t_app *app, t_vector2d *pos,
	t_vector2d *ray, t_vector2d *delta_dist)
{
	t_vector2d	side_dist;

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
static double	get_wall_distance(t_app *app, t_vector2d *pos, t_vector2d *ray)
{
	t_vector2d	side_dist;
	t_vector2d	delta_dist;
	int			side;

	delta_dist = (t_vector2d){fabs(1.f / ray->x), fabs(1.f / ray->y)};
	side_dist = get_side_distance(app, pos, ray, &delta_dist);
	// Start DDA
	while (1)
	{
		// Next map box (if x is closer go to x side...)
		if (side_dist.x < side_dist.y)
		{
			side_dist.x += delta_dist.x;
			if (ray->x < 0)
				pos->x += -1.f;
			else
				pos->x += 1.f;
			side = 0;
		}
		else
		{
			side_dist.y += delta_dist.y;
			if (ray->y < 0)
				pos->y += -1.f;
			else
				pos->y += 1.f;
			side = 1;
		}
		// Check hit
		if (app->map[(int) pos->y][(int) pos->x])
			break;
	}
	if (!side)
		return (side_dist.x - delta_dist.x);
	return (side_dist.y - delta_dist.y);	
}

/*
 * Renders the current view of the player.
*/
void	render_view(t_app *app)
{
	int			x;
	double		camera_x;
	t_vector2d	ray;
	t_vector2d	pos;

	x = 0;
	while (x < WIN_W)
	{
		camera_x = 2 * x / (double) WIN_W - 1.f;
		ray.x = app->player.direction.x + app->player.camera_plane.x * camera_x;
		ray.y = app->player.direction.y + app->player.camera_plane.y * camera_x;
		pos = (t_vector2d){app->player.position.x, app->player.position.y};
		draw_vertical_line(app, x,
			(int) (WIN_H / get_wall_distance(app, &pos, &ray)),
			app->map[(int) pos.y][(int) pos.x]);
		x++;
	}
	mlx_put_image_to_window(app->mlx, app->win, app->image->img, 0, 0);
}
