/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_render.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htahvana <htahvana@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 15:23:28 by saaltone          #+#    #+#             */
/*   Updated: 2022/07/26 13:54:35 by htahvana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	draw_object(t_app *app, int index, int screen_x, float depth)
{
	t_point	draw_start;
	t_point	draw_end;
	int		x;
	int		y;
	int		color;
	t_vector2	texture_step;
	t_vector2 texture_pixel;

	draw_start.x = -app->objects[index].width * 0.5f + screen_x;
	draw_start.y = -app->objects[index].height * 0.5f + WIN_H * 0.5f;
	draw_end.x = app->objects[index].width * 0.5f + screen_x;
	draw_end.y = app->objects[index].height * 0.5f + WIN_H * 0.5f;
	if (draw_start.x < 0)
		draw_start.x = 0;
	if (draw_start.y < 0)
		draw_start.y = 0;
	if (draw_end.x >= WIN_W)
		draw_end.x = WIN_W - 1;
	if (draw_end.y >= WIN_H)
		draw_end.y = WIN_H - 1;
	x = draw_start.x - 1;

	texture_step.x = TEX_SIZE / (double)(app->objects[index].width);
	texture_step.y = TEX_SIZE / (double)(app->objects[index].height);
	texture_pixel.x = 0.f;
	if (draw_end.x - draw_start.x < app->objects[index].width - 1 && draw_end.x < WIN_W - 1)
		texture_pixel.x = (draw_start.x - draw_end.x) * texture_step.x + 64.f;
	while (++x < draw_end.x)
	{
		y = draw_start.y;
		texture_pixel.y = 1;
		if (draw_end.y - draw_start.y < app->objects[index].height - 1 && draw_end.y < WIN_H - 1)
			texture_pixel.y = (draw_start.y - draw_end.y) * texture_step.y + 64.f;
		while (++y < draw_end.y)
		{
		color = get_pixel_color(app->object_sprites[app->objects[index].sprite_id].image, (int)texture_pixel.x - ((app->objects[index].frame_id) * TEX_SIZE) - TEX_SIZE, texture_pixel.y);
		if (color > 0)
			put_pixel_to_image_depth(app->image, app->depthmap, x, y, color, depth);
		texture_pixel.y += texture_step.y;
		}
		texture_pixel.x += texture_step.x;
	}
}