/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_render.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htahvana <htahvana@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 15:23:28 by saaltone          #+#    #+#             */
/*   Updated: 2022/08/03 16:52:24 by htahvana         ###   ########.fr       */
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

/* 
 * Object multithreaded rendering
 */
void	*render_objects(void *data)
{
	t_thread_data	*t;
	t_app			*app;
	int				i;
	t_vector2		dist;
	float			distance;
	t_vector2		transform;
	int				screen_x;
	double			rad;

	t = (t_thread_data *)data;
	app = (t_app *)t->app;
	i = -(t->id);
	while (i < app->object_count)
	{
		i += app->conf->thread_count;
		if (i < 0 || i >= app->object_count || app->objects[i].active == 0)
			continue ;
		dist.x = (app->objects[i].position.x - app->player.pos.x)
			* app->object_sprites[app->objects[i].sprite_id].offset_multiplier;
		dist.y = (app->objects[i].position.y - app->player.pos.y)
			* app->object_sprites[app->objects[i].sprite_id].offset_multiplier;
		transform = ft_vector_multiply_matrix(dist, ft_matrix_inverse((t_matrix2){
					app->player.cam,
					app->player.dir
				}));
		distance = ft_vector_length(dist);
		if ((transform.y / distance < 0.75f))
			continue ;
		rad = get_radial_direction(&dist);
		if (app->object_sprites[app->objects[i].sprite_id].mirrored)
			app->objects[i].frame_id = ((int)(rad * 64 / 180) % 64);
		else
			app->objects[i].frame_id = ((int)(rad * 64 / 360) % 64);
		if (app->objects[i].sprite_id < 2)
			app->objects[i].frame_id = app->object_sprites[app->objects[i].sprite_id].animation_step;
		screen_x = (int)((WIN_W / 2) * (1.0f + (transform.x / transform.y)));
		app->objects[i].width = abs((int)(WIN_H / transform.y));
		app->objects[i].height = abs((int)(WIN_H / transform.y));
		clamp_distance(&transform.y);
		app->objects[i].distance = transform.y;
		draw_object(app, &app->objects[i], screen_x);
	}
	pthread_exit(NULL);
}