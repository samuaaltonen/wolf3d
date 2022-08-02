/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_render.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htahvana <htahvana@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 15:23:28 by saaltone          #+#    #+#             */
/*   Updated: 2022/08/02 15:46:13 by htahvana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"


static void	limit_draw(t_point *start, t_point *end)
{
	if (start->x < 0)
		start->x = 0;
	if (start->y < 0)
		start->y = 0;
	if (end->x >= WIN_W)
		end->x = WIN_W - 1;
	if (end->y >= WIN_H)
		end->y = WIN_H - 1;
}

void	draw_object(t_app *app, int index, int screen_x, float depth)
{
	t_point	draw_start;
	t_point	draw_end;
	int		x;
	int		y;
	int		color;
	t_vector2	texture_step;
	t_vector2	texture_pixel;

	draw_start.x = -app->objects[index].width / 2 + screen_x;
	draw_start.y = -app->objects[index].height / 2 + WIN_H / 2;
	draw_end.x = app->objects[index].width / 2 + screen_x;
	draw_end.y = app->objects[index].height / 2 + WIN_H / 2;
	limit_draw(&draw_start, &draw_end);
	x = draw_start.x - 1;

	texture_step.x = TEX_SIZE / (double)(app->objects[index].width);
	texture_step.y = TEX_SIZE / (double)(app->objects[index].height);
	texture_pixel.x = 0.f;
	
	//ft_printf("%i\n", screen_x);
 	if (WIN_W < app->objects[index].width && draw_start.x == 0)
		{
			if(draw_end.x < WIN_W - 1)
				texture_pixel.x = ( app->objects[index].width - (draw_end.x - draw_start.x)) * (1.f - (screen_x / (float)(draw_end.x - draw_start.x))) * texture_step.x;
			else
				texture_pixel.x = ( app->objects[index].width - WIN_W) * (1.f - (screen_x / (float)(WIN_W))) * texture_step.x;
		} 
	else if (draw_end.x - draw_start.x < app->objects[index].width - 1 && draw_end.x < WIN_W - 1)
		{
		texture_pixel.x = (draw_start.x - draw_end.x) * texture_step.x + 64.f;
		}
	while (++x < draw_end.x)
	{
		y = draw_start.y;
		texture_pixel.y = 0.f;
 		if (draw_end.y - draw_start.y < app->objects[index].height - 1)
			texture_pixel.y = (draw_start.y - draw_end.y + app->objects[index].height) / 2 * texture_step.y;
		while (++y < draw_end.y)
		{
			color = get_pixel_color(app->object_sprites[app->objects[index].sprite_id].image, (int)texture_pixel.x - ((app->objects[index].frame_id) * TEX_SIZE) - TEX_SIZE, texture_pixel.y);
			if (color > 0)
				put_pixel_to_image_test(app->image, app->depthmap, x, y, color, depth);
			texture_pixel.y += texture_step.y;
		}
		texture_pixel.x += texture_step.x;
	}
}