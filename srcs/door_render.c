/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_render.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 13:45:02 by saaltone          #+#    #+#             */
/*   Updated: 2022/07/25 11:58:57 by saaltone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static double get_door_animation_step(t_app *app, int x, int y)
{
	int		i;

	i = -1;
	while (++i < app->door_count)
	{
		if ((int) app->doors[i].position.x == x && (int) app->doors[i].position.y == y)
		{
			if (app->doors[i].state == CLOSING)
				return (fabs(1.f - app->doors[i].animation_step));
			else
				return (app->doors[i].animation_step);
		}
	}
	return (0.f);
}

/*
 * Draws vertical line to image based on door distance (closer = higher)
*/
static void	draw_door(t_app *app, int x, int h, t_rayhit rayhit, double offset)
{
	int	start_pixel;
	int	end_pixel;
	int	i;
	double	y_step;
	double	tex_y;

	y_step = (TEX_SIZE / (double)h);
	tex_y = 0;
	start_pixel = WIN_H / 2 - h / 2;
	end_pixel = WIN_H / 2 + h / 2;
	start_pixel += (int)(offset * (double)(end_pixel - start_pixel));
	if (end_pixel >= WIN_H)
		end_pixel = WIN_H - 1;
	i = 0;
	if(rayhit.distance > MAX_RAY_DISTANCE)
		rayhit.distance = MAX_RAY_DISTANCE;
	rayhit.distance = 254 / MAX_RAY_DISTANCE * rayhit.distance + 1.f;
	h = end_pixel - start_pixel;
	while (i < h)
	{
		tex_y += y_step;
		if (start_pixel + i > 0)
			put_pixel_to_image(app->image, x, start_pixel + i, get_pixel_color(app->sprite, rayhit.tex_x + (DOOR_MAP_IDENTIFIER - 'A' - 1) * 64, (int)tex_y & (TEX_SIZE - 1)) | ((int)rayhit.distance << 24));
		i++;
	}
}

void	render_moving_doors(t_app *app)
{
	int			x;
	t_rayhit	rayhit;

	app->conf->render_moving_doors = 1;
	x = -1;
	while (++x < WIN_W)
	{
		if(!raycast(app, x, &rayhit))
			continue;
		if (app->map[(int) rayhit.hit_pos.y][(int) rayhit.hit_pos.x][0] != DOOR_MAP_IDENTIFIER_MOVING)
			continue;
		draw_door(app, x, (int)(WIN_H / rayhit.distance), rayhit, 
			get_door_animation_step(app,
				(int) rayhit.hit_pos.x,
				(int) rayhit.hit_pos.y));
	}
	app->conf->render_moving_doors = 0;
}
