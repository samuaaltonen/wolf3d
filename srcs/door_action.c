/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_action.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 13:45:02 by saaltone          #+#    #+#             */
/*   Updated: 2022/07/22 16:49:21 by saaltone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/**
 * Updates map depending on door state. If door is open or moving, "hides" it 
 * from wall rendering.
*/
static void	door_update(t_app *app, int index, t_doorstate state)
{
	app->doors[index].state = state;
	if (state == CLOSED)
		app->map[(int) app->doors[index].position.y][(int) app->doors[index].position.x][0] = DOOR_MAP_IDENTIFIER;
	else
		app->map[(int) app->doors[index].position.y][(int) app->doors[index].position.x][0] = DOOR_MAP_IDENTIFIER_MOVING;
}

/**
 * Handles door action and opens or closes it depending on the door state.
*/
void	door_action(t_app *app)
{
	int		i;
	double	distance;

	i = -1;
	while (++i < app->door_count)
	{
		if (app->doors[i].state != CLOSED)
			continue;
		distance = ft_vector_length((t_vector2){
			app->doors[i].position.x - app->player.position.x,
			app->doors[i].position.y - app->player.position.y
		});
		if (distance > DOOR_ACTION_DISTANCE_THRESHOLD)
			continue;
		if (app->doors[i].state == CLOSED)
			door_update(app, i, OPENING);
		app->doors[i].animation_begin = clock();
		app->doors[i].animation_step = 0.f;
	}
}

/**
 * Monitors door states and progresses their animation steps when necessary.
*/
void	progress_doors(t_app *app)
{
	double	animation_increase;
	int		i;

	app->conf->has_moving_doors = 0;
	animation_increase = app->conf->delta_time / DOOR_ANIMATION_DURATION;
	i = -1;
	while (++i < app->door_count)
	{
		if (app->doors[i].state == OPEN
			&& app->doors[i].animation_begin < clock() - DOOR_CLOSING_THRESHOLD * CLOCKS_PER_SEC
			&& ((int) app->player.position.x != (int) app->doors[i].position.x
			|| (int) app->player.position.y != (int) app->doors[i].position.y))
		{
			door_update(app, i, CLOSING);
			app->doors[i].animation_begin = clock();
			app->doors[i].animation_step = 0.f;
		}
		if (app->doors[i].state != OPENING && app->doors[i].state != CLOSING)
			continue;
		if (app->doors[i].state == OPENING && app->doors[i].animation_step >= 1.f)
			door_update(app, i, OPEN);
		if (app->doors[i].state == CLOSING && app->doors[i].animation_step >= 1.f)
			door_update(app, i, CLOSED);
		app->doors[i].animation_step += animation_increase;
		app->conf->has_moving_doors = 1;
	}
}
