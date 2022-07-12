/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 15:23:28 by saaltone          #+#    #+#             */
/*   Updated: 2022/07/12 17:00:30 by saaltone         ###   ########.fr       */
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
					app->map[y][x][3] - '0'
				};
				i++;
			}
		}
	}
	app->object_count = i;
}

void	draw_object(t_app *app, int screen_x, int height)
{
	// WORK IN PROGRESS
	(void)app;
	(void)screen_x;
	(void)height;
}

void	cast_objects(t_app *app)
{
	t_vector2	dist;
	t_vector2	transform;
	int			i;
	int			screen_x;
	int			height;

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
		height = abs(WIN_H / (int) transform.y);
		draw_object(app, screen_x, height);
		i++;
	}
}