/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 14:32:45 by saaltone          #+#    #+#             */
/*   Updated: 2022/06/09 11:51:46 by saaltone         ###   ########.fr       */
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
 * Renders the current view of the player.
*/
void	render_view(t_app *app)
{
	int			x;
	double		camera_x;
	t_vector2d	ray;
	t_vector2d	map_pos;
	t_vector2d	side_distance;
	t_vector2d	delta_distance;
	t_vector2d	step;
	double		wall_distance;
	int			side;

	x = 0;
	while (x < WIN_W)
	{
		// Calculate ray position and direction
		// camera x (-1 left side of the screen, 0 center, 1 right)
		camera_x = 2 * x / (double) WIN_W - 1.f;
		ray.x = app->player.direction.x + app->player.camera_plane.x * camera_x;
		ray.y = app->player.direction.y + app->player.camera_plane.y * camera_x;

		// Start raycasting from current position
		map_pos = (t_vector2d){app->player.position.x, app->player.position.y};

		// Calculate ray lenght from one side to next side
		if (ray.y == 0.f)
			ray.y = 0.0009765625f;
		delta_distance = (t_vector2d){fabs(1.f / ray.x), fabs(1.f / ray.y)};

		// Step and side distance
		if (ray.x < 0)
		{
			step.x = -1;
			side_distance.x = (app->player.position.x - map_pos.x) * delta_distance.x;
		}
		else
		{
			step.x = 1;
			side_distance.x = (map_pos.x + 1.f - app->player.position.x) * delta_distance.x;
		}
		if (ray.y < 0)
		{
			step.y = -1;
			side_distance.y = (app->player.position.y - map_pos.y) * delta_distance.y;
		}
		else
		{
			step.y = 1;
			side_distance.y = (map_pos.y + 1.f - app->player.position.y) * delta_distance.y;
		}

		// Start DDA
		while (1)
		{
			// Next map box (if x is closer go to x side...)
			if (side_distance.x < side_distance.y)
			{
				side_distance.x += delta_distance.x;
				map_pos.x += step.x;
				side = 0;
			}
			else
			{
				side_distance.y += delta_distance.y;
				map_pos.y += step.y;
				side = 1;
			}
			//ft_printf("Checking DDA for %f, %f\n", map_pos.x, map_pos.y);
			// Check hit
			if (app->map[(int) map_pos.y][(int) map_pos.x])
				break;
		}
		//ft_printf("x: %i, wall hit at %f, %f\n", x, map_pos.x, map_pos.y);

		// Calculate distance of the ray to the wall depending on which side
		// it hit (x or y side)
		if (!side)
			wall_distance = side_distance.x - delta_distance.x;
		else
			wall_distance = side_distance.y - delta_distance.y;

		// Draw line
		//ft_printf("wall distance: %f, ray y: %f\n", wall_distance, ray.y);
		draw_vertical_line(app, x, (int) (WIN_H / wall_distance), app->map[(int) map_pos.y][(int) map_pos.x]);
		x++;
	}
	mlx_put_image_to_window(app->mlx, app->win, app->image->img, 0, 0);
}
