/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_action.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 11:34:41 by htahvana          #+#    #+#             */
/*   Updated: 2022/08/01 12:45:13 by saaltone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"


// Checks if the player is in the same position as a
void coin_grab(t_app *app)
{
	int	i;

	i = -1;
	while (++i < app->object_count)
	{
		if (app->objects[i].active && (app->objects[i].sprite_id == 0 || app->objects[i].sprite_id == 1) && (int)app->player.pos.x ==  (int)app->objects[i].position.x && (int)app->player.pos.y ==  (int)app->objects[i].position.y)
		{
			if (ft_vector_length((t_vector2){app->player.pos.x - app->objects[i].position.x , app->player.pos.y - app->objects[i].position.y}) < 0.5f)
			{
				app->objects[i].active = 0;
				app->conf->coin_points++;
			}
		}
	}
}