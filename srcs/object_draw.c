/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_draw.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htahvana <htahvana@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 11:51:00 by htahvana          #+#    #+#             */
/*   Updated: 2022/08/04 12:05:43 by htahvana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/**
 * Initializes draw area depending on object position/size and limits it to
 * window dimensions.
*/
static void	init_draw_area(t_point *draw_start, t_point *draw_end,
	t_object *object, int screen_x)
{
	draw_start->x = -object->width / 2 + screen_x;
	draw_start->y = -object->height / 2 + WIN_H / 2;
	draw_end->x = object->width / 2 + screen_x;
	draw_end->y = object->height / 2 + WIN_H / 2;
	if (draw_start->x < 0)
		draw_start->x = 0;
	if (draw_start->y < 0)
		draw_start->y = 0;
	if (draw_end->x >= WIN_W)
		draw_end->x = WIN_W - 1;
	if (draw_end->y >= WIN_H)
		draw_end->y = WIN_H - 1;
}

/**
 * Offsets object texture when necessary (i.e. when near window border or when
 * object is larger than window).
*/
static void	check_texture_offset(t_point *draw_start, t_point *draw_end,
	t_object *object, int screen_x)
{
	t_vector2	offset;
	t_vector2	step;

	offset.x = 0.f;
	offset.y = 0.f;
	step.x = TEX_SIZE / (double)(object->width);
	step.y = TEX_SIZE / (double)(object->height);
	if (WIN_W < object->width && draw_start->x == 0)
	{
		if (draw_end->x < WIN_W - 1)
			offset.x = (object->width - (draw_end->x - draw_start->x))
				* (1.f - (screen_x / (double)(draw_end->x - draw_start->x)))
				* step.x;
		else
			offset.x = (object->width - WIN_W)
				* (1.f - (screen_x / (double)(WIN_W))) * step.x;
	}
	else if (draw_end->x - draw_start->x < object->width - 1
		&& draw_end->x < WIN_W - 1)
		offset.x = (draw_start->x - draw_end->x) * step.x + (double)TEX_SIZE;
	if (draw_end->y - draw_start->y < object->height - 1)
		offset.y = (draw_start->y - draw_end->y + object->height) / 2 * step.y;
	object->texture_offset = offset;
}

static void	draw_object_pixel(t_app *app, t_object *object,
	t_point *window_pixel, t_vector2 *texture_pixel)
{
	int	color;

	color = get_pixel_color(app->object_sprites[object->sprite_id].image,
			(int)texture_pixel->x - ((object->frame_id) * TEX_SIZE) - TEX_SIZE,
			(int)texture_pixel->y);
	if (color > 0)
		put_pixel_to_image_test(app, (t_point){window_pixel->x,
			window_pixel->y}, color, object->distance);
}

/**
 * Draws an object to image at specific x position with specific size depending
 * on the distance.
*/
void	draw_object(t_app *app, t_object *object, int screen_x)
{
	t_point		draw_start;
	t_point		draw_end;
	t_point		window_pixel;
	t_vector2	texture_step;
	t_vector2	texture_pixel;

	init_draw_area(&draw_start, &draw_end, object, screen_x);
	texture_step.x = TEX_SIZE / (double)(object->width);
	texture_step.y = TEX_SIZE / (double)(object->height);
	check_texture_offset(&draw_start, &draw_end, object, screen_x);
	window_pixel.x = draw_start.x - 1;
	texture_pixel.x = object->texture_offset.x;
	while (++window_pixel.x < draw_end.x)
	{
		window_pixel.y = draw_start.y;
		texture_pixel.y = object->texture_offset.y;
		while (++window_pixel.y < draw_end.y)
		{
			draw_object_pixel(app, object, &window_pixel, &texture_pixel);
			texture_pixel.y += texture_step.y;
		}
		texture_pixel.x += texture_step.x;
	}
}
