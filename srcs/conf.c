/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htahvana <htahvana@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 15:14:06 by saaltone          #+#    #+#             */
/*   Updated: 2022/08/04 15:56:22 by htahvana         ###   ########.fr       */
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
	int	height_per_thread;

	i = 0;
	width_per_thread = WIN_W / THREAD_COUNT;
	height_per_thread = (WIN_H * 0.5f) / THREAD_COUNT;
	while (i < THREAD_COUNT)
	{
		app->thread_info[i] = (t_thread_data){
			app,
			width_per_thread * i,
			width_per_thread * (i + 1) - 1,
			height_per_thread * i + WIN_H * 0.5f + 1,
			height_per_thread * (i + 1) + WIN_H * 0.5f,
			i + 1
		};
		i++;
	}
	app->thread_info[i - 1].y_end = WIN_H - 1;
	app->thread_info[i - 1].x_end = WIN_W - 1;
}

/**
 * Calculates initial camera plane using FOV and player direction vector.
 * Camera plane is always perpendicular to the direction vector.
 * 
 * cameralength = directionlength * tan(FOV / 2)
*/
void	init_camera_plane(t_app *app)
{
	t_vector2	camera_plane;
	double		camera_length;
	double		direction_length;

	direction_length = ft_vector_length(app->player.dir);
	camera_length = direction_length * tan(app->conf->fov
			* DEG_IN_RADIAN / 2.0f);
	camera_plane = ft_vector_resize(ft_vector_perpendicular(
				app->player.dir), camera_length);
	app->player.cam = camera_plane;
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
	ft_bzero(app->conf,sizeof(t_conf));
	app->conf->toggle_bloom = 1;
	app->conf->fps_count = 1;
	clock_gettime(CLOCK_REALTIME, &app->conf->fps_clock);
	app->conf->fov = FOV;
	app->conf->skybox_offset = 360.f;
	app->conf->movement_speed = MOVEMENT_SPEED;
	app->conf->rotation_speed = ROTATION_SPEED;
	init_thread_info(app);
	init_objects(app);
	init_doors(app);
	return (1);
}
