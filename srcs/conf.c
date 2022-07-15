/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 15:14:06 by saaltone          #+#    #+#             */
/*   Updated: 2022/07/15 14:19:28 by saaltone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/**
 * Inits thread information structs. They contain info about what part of the
 * window each thread calculates.
*/
void	init_thread_info(t_app *app)
{
	int	i;
	int	width_per_thread;

	i = 0;
	width_per_thread = WIN_W / app->conf->thread_count;
	while (i < app->conf->thread_count && i < THREADS_MAX)
	{
		app->thread_info[i] = (t_thread_data){
			app,
			width_per_thread * i,
			width_per_thread * (i + 1),
		};
		i++;
	}
	app->thread_info[i].x_end = WIN_W - 1;
}

/**
 * Calculates initial camera plane using FOV and player direction vector.
 * Camera plane is always perpendicular to the direction vector.
 * 
 * cameralenght = directionlenght * tan(FOV / 2)
*/
void	init_camera_plane(t_app *app)
{
	t_vector2	camera_plane;
	double		camera_length;
	double		direction_length;

	direction_length = ft_vector_length(app->player.direction);
	camera_length = direction_length * tan(app->conf->fov
			* DEG_IN_RADIAN / 2.0f);
	camera_plane = ft_vector_resize(ft_vector_perpendicular(
				app->player.direction), camera_length);
	app->player.camera_plane = camera_plane;
}

/**
 * Initializes configuration struct.
*/
int	conf_init(t_app *app)
{
	if (!app)
		return (0);
	app->conf = (t_conf *)malloc(sizeof(t_conf));
	if (!(app->conf))
		return (0);
	app->conf->win_name = WIN_NAME;
	app->conf->toggle_help = 0;
	app->conf->win_w = WIN_W;
	app->conf->win_h = WIN_H;
	app->conf->thread_count = THREADS_DEFAULT;
	app->conf->fov = FOV;
	init_thread_info(app);
	init_objects(app);
	return (1);
}
