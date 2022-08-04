/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_render.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htahvana <htahvana@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 15:23:28 by saaltone          #+#    #+#             */
/*   Updated: 2022/08/04 15:56:32 by htahvana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void	get_frame(t_app *app, t_vector2 *dist, int i)
{
	double	rad;

	rad = get_radial_direction(dist);
	if (app->object_sprites[app->objects[i].sprite_id].mirrored)
		app->objects[i].frame_id = ((int)(rad * 64 / 180) % 64);
	else
		app->objects[i].frame_id = ((int)(rad * 64 / 360) % 64);
	if (app->objects[i].sprite_id < 2)
		app->objects[i].frame_id
			= (int)app->object_sprites[app->objects[i].sprite_id].animation_step;
}

static double	object_distance(t_app *app, t_vector2 *dist,
	t_vector2 *transform, int i)
{
	dist->x = (app->objects[i].position.x - app->player.pos.x)
		* app->object_sprites[app->objects[i].sprite_id].offset_multiplier;
	dist->y = (app->objects[i].position.y - app->player.pos.y)
		* app->object_sprites[app->objects[i].sprite_id].offset_multiplier;
	*transform = ft_vector_multiply_matrix(*dist,
			ft_matrix_inverse((t_matrix2){
				app->player.cam,
				app->player.dir
			}));
	return (transform->y / ft_vector_length(*dist));
}

/* 
 * Object multithreaded rendering
 */
void	*render_objects(void *data)
{
	t_app			*app;
	int				i;
	t_vector2		dist;
	t_vector2		transform;

	app = (t_app *)((t_thread_data *)data)->app;
	i = (((t_thread_data *)data)->id) - 1;
	while (i < app->object_count)
	{
		if (app->objects[i].active == 0
			|| object_distance(app, &dist, &transform, i) < 0.75f)
		{
			i += THREAD_COUNT;
			continue ;
		}
		app->objects[i].width = abs((int)(WIN_H / transform.y));
		app->objects[i].height = abs((int)(WIN_H / transform.y));
		clamp_distance(&transform.y);
		app->objects[i].distance = transform.y;
		get_frame(app, &dist, i);
		draw_object(app, &app->objects[i], (int)((WIN_W / 2)
				* (1.0f + (transform.x / transform.y))));
		i += THREAD_COUNT;
	}
	pthread_exit(NULL);
}
