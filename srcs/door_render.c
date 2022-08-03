/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_render.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htahvana <htahvana@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 13:45:02 by saaltone          #+#    #+#             */
/*   Updated: 2022/08/03 11:55:31 by htahvana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/**
 * Returns vertical offset for the door depending on its animation state.
*/
static double	get_door_offset(t_app *app, t_vector2 hit_pos, int start_pixel,
	int end_pixel)
{
	int		i;
	double	step;

	step = 0.f;
	i = -1;
	while (++i < app->door_count)
	{
		if ((int) app->doors[i].position.x == (int) hit_pos.x
			&& (int) app->doors[i].position.y == (int) hit_pos.y)
		{
			step = app->doors[i].animation_begin / DOOR_ANIMATION_DURATION;
			if (app->doors[i].state == CLOSING)
				step = fabs(1.f - step);
			break ;
		}
	}
	return (step * (double)(end_pixel - start_pixel));
}

/*
 * Draws vertical line to image based on door distance (closer = higher)
*/
static void	draw_door_vertical_line(t_app *app, int x, int h, t_rayhit rayhit)
{
	int		start_pixel;
	int		end_pixel;
	int		i;
	double	y_step;
	int		texture_x;

	y_step = (TEX_SIZE / (double)h);
	texture_x = rayhit.tex_x + (DOOR_MAP_ID - 'A' - 1) * TEX_SIZE;
	start_pixel = WIN_H / 2 - h / 2;
	end_pixel = WIN_H / 2 + h / 2;
	start_pixel += get_door_offset(app, rayhit.hit_pos, start_pixel, end_pixel);
	if (end_pixel >= WIN_H)
		end_pixel = WIN_H - 1;
	i = 0;
	clamp_distance(&rayhit.distance);
	h = end_pixel - start_pixel;
	while (i < h)
	{
		if (start_pixel + i > 0)
			put_pixel_to_image_test(app->image, app->depthmap, x,
				start_pixel + i,
				get_pixel_color(app->sprite, texture_x, (int)(y_step * i)),
				rayhit.distance);
		i++;
	}
}

/**
 * Draws all moving doors using raycasting.
*/
void	render_moving_doors(t_app *app)
{
	int			x;
	t_rayhit	rayhit;

	app->conf->render_moving_doors = 1;
	x = -1;
	while (++x < WIN_W)
	{
		if (!raycast(app, x, &rayhit))
			continue ;
		if (app->map[(int) rayhit.hit_pos.y][(int) rayhit.hit_pos.x][0]
			!= DOOR_MAP_ID_MOVING)
			continue ;
		draw_door_vertical_line(app, x, (int)(WIN_H / rayhit.distance), rayhit);
	}
	app->conf->render_moving_doors = 0;
}
