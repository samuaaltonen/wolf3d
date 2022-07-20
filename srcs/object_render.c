/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_render.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htahvana <htahvana@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 15:23:28 by saaltone          #+#    #+#             */
/*   Updated: 2022/07/20 13:50:26 by htahvana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	draw_object(t_app *app, t_vector2 *transform, int index, int screen_x, int depth)
{
	t_point	draw_start;
	t_point	draw_end;
	//t_point	texture_pixel;
	int		x;
	int		y;
	int		color;
	t_vector2	texture_step;
	t_vector2 texture_pixel;

	draw_start.x = -app->objects[index].width / 2 + screen_x;
	draw_start.y = -app->objects[index].height / 2 + WIN_H / 2;
	draw_end.x = app->objects[index].width / 2 + screen_x;
	draw_end.y = app->objects[index].height / 2 + WIN_H / 2;
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
	texture_step.y = TEX_SIZE / (double)(app->objects[index].width);

	texture_pixel.x = 0;
	if(draw_end.x - draw_start.x < app->objects[index].width && draw_end.x < WIN_W - 1)
		texture_pixel.x = (draw_start.x - draw_end.x) * texture_step.x;
	while (++x < draw_end.x)
	{
		y = draw_start.y;
		texture_pixel.y = 0;
		if (transform->y > 0)
		{
			while(++y < draw_end.y)
			{
			color = get_pixel_color(app->object_sprites[app->objects[index].sprite_id].image, texture_pixel.x + app->object_sprites[app->objects[index].sprite_id].animation_step * TEX_SIZE, texture_pixel.y);
				if (color > 0)
					put_pixel_to_image_depth(app->image, x, y, color | (depth << 24));
			texture_pixel.y += texture_step.y;
		}
		}
		texture_pixel.x += texture_step.x;
	}
}

/* void	cast_objects(t_app *app)
{
	t_vector2	dist;
	t_vector2	transform;
	int			i;
	int			screen_x;
	double		distance;

	i = 0;
	while (i < app->object_count)
	{
		dist.x = (app->objects[i].position.x - app->player.position.x) * app->object_sprites[app->objects[i].sprite_id].offset_multiplier;
		dist.y = (app->objects[i].position.y - app->player.position.y) * app->object_sprites[app->objects[i].sprite_id].offset_multiplier;
		transform = ft_vector_multiply_matrix(dist, ft_matrix_inverse((t_matrix2){
			app->player.camera_plane,
			app->player.direction
		}));
		distance = ft_vector_length(dist);
		if(distance > MAX_RAY_DISTANCE)
			distance = MAX_RAY_DISTANCE;
		screen_x = (int)((WIN_W / 2) * (1.0f + (transform.x / transform.y)));

		app->objects[i].width = abs((int)(WIN_H / transform.y));
		app->objects[i].height = abs((int)(WIN_H / transform.y));
		draw_object(app, &transform, i, screen_x, (int)(255 / MAX_RAY_DISTANCE * distance));
		i++;
	}
} */