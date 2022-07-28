/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htahvana <htahvana@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 14:11:42 by saaltone          #+#    #+#             */
/*   Updated: 2022/07/28 15:04:46 by htahvana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/**
 * Calculates door count in the map.
*/
static int	get_door_count(t_app *app)
{
	int	x;
	int	y;
	int	count;

	y = 0;
	count = 0;
	while (y < app->map_size.y)
	{
		x = 0;
		while (x < app->map_size.x)
		{
			if (app->map[y][x][0] == DOOR_MAP_IDENTIFIER)
				count++;
			x++;
		}
		y++;
	}
	return (count);
}

/**
 * Reads all doors from the map and initilizes an array containing positions
 * of the doors and initial states.
*/
void	init_doors(t_app *app)
{
	int	x;
	int	y;
	int	i;

	app->doors = (t_door*)malloc(sizeof(t_door) * (get_door_count(app) + 1));
	if (!app->doors)
		exit_error(MSG_ERROR_ALLOC);
	y = -1;
	i = 0;
	while (++y < app->map_size.y)
	{
		x = -1;
		while (++x < app->map_size.x)
		{
			if (app->map[y][x][0] == DOOR_MAP_IDENTIFIER)
			{
				app->doors[i] = (t_door){
					(t_vector2){(double)x + 0.5f, (double)y + 0.5f},
					CLOSED,
					0.f
				};
				i++;
			}
		}
	}
	app->door_count = i;
}
