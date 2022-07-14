/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 15:13:49 by saaltone          #+#    #+#             */
/*   Updated: 2022/07/14 15:49:10 by saaltone         ###   ########.fr       */
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
	while (y < app->map_size.y)
	{
		x = 0;
		while (x < app->map_size.x)
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
	while (++y < app->map_size.y)
	{
		x = -1;
		while (++x < app->map_size.x)
		{
			if (app->map[y][x][0] == '0' && app->map[y][x][3] > '0')
			{
				app->objects[i] = (t_object){
					(t_vector2){(double)x + 0.5f, (double)y + 0.5f},
					app->map[y][x][3] - '0', 0, 0
				};
				i++;
			}
		}
	}
	app->object_count = i;
}

/**
 * Loads object textures into memory.
*/
void	load_object_textures(t_app *app)
{
	int					i;
	static const char	*texture_ids[MAP_MAX_OBJECT_IDS] = {
		"./coin_spritesheet.xpm"
	};

	i = 0;
	while (i < MAP_MAX_OBJECT_IDS && texture_ids[i])
	{
		app->object_sprites[i + 1] = init_xpm_image(app->mlx, TEX_SIZE * 64, TEX_SIZE, (char *)texture_ids[i]);
		if (!(app->object_sprites[i + 1]))
			exit_error(MSG_ERROR_TEXTURE_FILE_ACCESS);
		i++;
	}
}
