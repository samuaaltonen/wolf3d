/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_render.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/08 17:01:42 by saaltone          #+#    #+#             */
/*   Updated: 2022/08/08 17:26:03 by saaltone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/**
 * Sets starting pixel, wall height and texture step limited to window
 * dimensions.
 */
static void	wall_limits(int *start_pixel, int *height,
	double *y_step, double *tex_y)
{
	*height += 2;
	*y_step = (TEX_SIZE / (double)*height);
	*tex_y = 0;
	if (*height > WIN_H)
	{
		*tex_y = (*height - WIN_H) / 2 * *y_step;
		*height = WIN_H;
	}
	*start_pixel = WIN_H / 2 - *height / 2;
	if (*start_pixel < 0)
		*start_pixel = 0;
}

/**
 * Draws vertical line to image based on wall distance (closer = higher).
 */
static void	draw_vertical_line(t_app *app, int x, int height, t_rayhit rayhit)
{
	int		start_pixel;
	int		i;
	double	y_step;
	double	tex_y;

	wall_limits(&start_pixel, &height, &y_step, &tex_y);
	i = 0;
	clamp_distance(&rayhit.distance);
	while (i < height)
	{
		tex_y += y_step;
		if (rayhit.type == EMPTY_MAP_ID)
			put_pixel_to_image_depth(app, (t_point){x, start_pixel + i},
				0, rayhit.distance);
		else
			put_pixel_to_image_depth(app, (t_point){x, start_pixel + i},
				get_pixel_color(app->sprite, rayhit.tex_x + (rayhit.type - 'A')
					* 64, (int)tex_y & (TEX_SIZE - 1)), rayhit.distance);
		i++;
	}
}

/**
 * Renders walls.
 */
void	*render_walls(void *data)
{
	t_thread_data	*t;
	t_app			*app;
	int				x;
	t_rayhit		rayhit;

	t = (t_thread_data *)data;
	app = (t_app *)t->app;
	x = t->x_start - 1;
	while (++x <= t->x_end)
	{
		raycast(app, x, &rayhit);
		draw_vertical_line(app, x, (int)(WIN_H / rayhit.distance), rayhit);
	}
	pthread_exit(NULL);
}
