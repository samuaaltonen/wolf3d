/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_action.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 13:45:02 by saaltone          #+#    #+#             */
/*   Updated: 2022/07/22 14:40:46 by saaltone         ###   ########.fr       */
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
		app->map[(int) app->doors[index].position.y][(int) app->doors[index].position.x][0] = EMPTY_MAP_IDENTIFIER;
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
		distance = ft_vector_length((t_vector2){
			app->doors[i].position.x - app->player.position.x,
			app->doors[i].position.y - app->player.position.y
		});
		if (distance > DOOR_ACTION_DISTANCE_THRESHOLD || distance < 0.5f)
			continue;
		if (app->doors[i].state == CLOSED)
			door_update(app, i, OPENING);
		if (app->doors[i].state == OPEN)
			door_update(app, i, CLOSING);
		app->doors[i].animation_begin = clock();
		app->doors[i].animation_step = 0.f;
		ft_printf("Action on door at (%f, %f). New state: %d\n", app->doors[i].position.x, app->doors[i].position.y, app->doors[i].state);
	}
}

/**
 * Monitors door states and progresses their animation steps when necessary.
*/
void	progress_doors(t_app *app)
{
	double	animation_increase;
	int		i;

	animation_increase = app->conf->delta_time / DOOR_ANIMATION_DURATION;
	i = -1;
	while (++i < app->door_count)
	{
		if (app->doors[i].state == OPEN && app->doors[i].animation_begin < clock() - DOOR_CLOSING_THRESHOLD * CLOCKS_PER_SEC)
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
		ft_printf("Animation on door at (%f, %f). Progress: %03.2f %%\n", app->doors[i].position.x, app->doors[i].position.y, app->doors[i].animation_step * 100);
	}
}
