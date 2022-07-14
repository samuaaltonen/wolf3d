/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 15:23:28 by saaltone          #+#    #+#             */
/*   Updated: 2022/07/14 13:42:47 by saaltone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/**
 * Calculates object count in the map.
*/
static int	get_object_count(t_app *app)
{
	int	x;
	int	y;
	int	object_count;

	y = 0;
	object_count = 0;
	while (y < MAP_HEIGHT)
	{
		x = 0;
		while (x < MAP_WIDTH)
		{
			if (app->map[y][x][0] == '0' && app->map[y][x][3] > '0')
				object_count++;
			x++;
		}
		y++;
	}
	return (object_count);
}

/**
 * Reads all objects from the map and initilizes an array containing positions
 * of the objects and their textures.
*/
void	init_objects(t_app *app)
{
	int	x;
	int	y;
	int	i;

	app->objects = (t_object*)malloc(sizeof(t_object) * (get_object_count(app) + 1));
	if (!app->objects)
		exit_error(MSG_ERROR_ALLOC);
	y = -1;
	i = 0;
	while (++y < MAP_HEIGHT)
	{
		x = -1;
		while (++x < MAP_WIDTH)
		{
			if (app->map[y][x][0] == '0' && app->map[y][x][3] > '0')
			{
				app->objects[i] = (t_object){
					(t_vector2){(double)x, (double)y},
					app->map[y][x][3] - '0', 0, 0
				};
				i++;
			}
		}
	}
	app->object_count = i;
}

void	draw_object(t_app *app, t_vector2 *transform, int index, int screen_x)
{
	t_point	draw_start;
	t_point	draw_end;
	t_point	texture_pixel;
	int		x;
	int		y;
	int		color;

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
	x = draw_start.x;
	while (x < draw_end.x)
	{
		texture_pixel.x = (x - (-app->objects[index].width / 2 + screen_x)) * TEX_SIZE / app->objects[index].width;
		y = draw_start.y;
		if (transform->y > 0 && transform->y < app->distance_buffer[x])
			while(y < draw_end.y)
			{
				texture_pixel.y = (y - WIN_H / 2+ app->objects[index].height / 2) * TEX_SIZE / app->objects[index].height;
				color = get_pixel_color(app->coin, texture_pixel.x, texture_pixel.y);
				if (color > 0)
					put_pixel_to_image(app->image, x, y, get_pixel_color(app->coin, texture_pixel.x, texture_pixel.y));
				y++;
			}
		x++;
	}
}

void	cast_objects(t_app *app)
{
	t_vector2	dist;
	t_vector2	transform;
	int			i;
	int			screen_x;

	i = 0;
	while (i < app->object_count)
	{
		dist.x = app->objects[i].position.x - app->player.position.x;
		dist.y = app->objects[i].position.y - app->player.position.y;
		transform = ft_vector_multiply_matrix(dist, ft_matrix_inverse((t_matrix2){
			app->player.camera_plane,
			app->player.direction
		}));
		screen_x = (int)((WIN_W / 2) * (1.0f + (transform.x / transform.y)));
		app->objects[i].width = abs((int)(WIN_H / transform.y));
		app->objects[i].height = abs((int)(WIN_H / transform.y));
		draw_object(app, &transform, i, screen_x);
		i++;
	}
}