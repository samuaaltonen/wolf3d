/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htahvana <htahvana@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 15:13:49 by saaltone          #+#    #+#             */
/*   Updated: 2022/07/20 16:50:46 by htahvana         ###   ########.fr       */
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

	y = -1;
	object_count = 0;
	while (++y < app->map_size.y)
	{
		x = -1;
		while (++x < app->map_size.x)
		{
			if (app->map[y][x][0] == 'A' && app->map[y][x][3] > 'A')
				object_count++;
		}
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
			if (app->map[y][x][0] == 'A' && app->map[y][x][3] > 'A')
			{
				app->objects[i] = (t_object){
					(t_vector2){(double)x + 0.5f, (double)y + 0.5f},
					app->map[y][x][3] - 'A' - 1, 0, 0, 0
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
	int							i;
	static const t_sprite_data	sprite_infos[] = {
		{TEXTURE_COIN_SPIN, NULL, 1.0f, 0, 64},
		{TEXTURE_COIN_WHIRL, NULL, 1.0f, 0, 64},
		{TEXTURE_PILLAR, NULL, 1.0f, 0, 64},
		{TEXTURE_CANNON, NULL, 1.0f, 0, 64},
		{NULL, NULL, 0, 0, 0}
	};

	i = 0;
	while (i < MAP_MAX_OBJECT_IDS && sprite_infos[i].path)
	{
		app->object_sprites[i] = (t_sprite_data) {
			sprite_infos[i].path,
			init_xpm_image(app->mlx, TEX_SIZE, TEX_SIZE, sprite_infos[i].path),
			sprite_infos[i].offset_multiplier,
			sprite_infos[i].animation_step,
			0
		};
		if (!(app->object_sprites[i].image))
			exit_error(MSG_ERROR_TEXTURE_FILE_ACCESS);
		app->object_sprites[i].total_steps = app->object_sprites[i].image->width / TEX_SIZE;
		i++;
	}
}
