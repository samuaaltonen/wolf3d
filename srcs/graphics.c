/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 14:32:45 by saaltone          #+#    #+#             */
/*   Updated: 2022/07/12 15:56:31 by saaltone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
 * Draws vertical line to image based on wall distance (closer = higher)
*/
static void	draw_vertical_line(t_app *app, int x, int height, t_rayhit rayhit)
{
	int	start_pixel;
	int	end_pixel;
	int	i;
	double	y_step;
	double	tex_y;

	y_step = (TEX_SIZE / (double)height);

	tex_y = 0;
	if (height > WIN_H)
	{
		tex_y = (height - WIN_H) / 2 * y_step;
		height = WIN_H;
	}
	start_pixel = WIN_H / 2 - height / 2;
	end_pixel = WIN_H / 2 + height / 2;
	if (start_pixel < 0)
		start_pixel = 0;
	if (end_pixel >= WIN_H)
		end_pixel = WIN_H - 1;
	i = 0;
	while (i < height)
	{
		tex_y += y_step;
		if(1) //toggle for cardinal texturing
			put_pixel_to_image(app->image, x, start_pixel + i, get_pixel_color(app->sprite, rayhit.tex_x + (rayhit.type - '0') * 64, (int)tex_y & (TEX_SIZE - 1)));
		else
			put_pixel_to_image(app->image, x, start_pixel + i, get_pixel_color(app->sprite, rayhit.tex_x + rayhit.direction * 64, (int)tex_y & (TEX_SIZE - 1)));

		i++;
	}
}

/*
 * Draws horizontal scanline for the floor
*/
static void	draw_horizontal_line(t_app *app, int y, t_vector2 *step, t_vector2 *floor_pos)
{

	t_point		texture_coord;
	t_point		coord;
	int			x;

	x = 0;

		while (x++ < WIN_W)
		{
			
			coord = (t_point){(int)floor_pos->x, (int)floor_pos->y};
			texture_coord.x = (int)(TEX_SIZE * (floor_pos->x - coord.x)) & (TEX_SIZE - 1);
			texture_coord.y = (int)(TEX_SIZE * (floor_pos->y - coord.y)) & (TEX_SIZE - 1);
			floor_pos->x += step->x;
			floor_pos->y += step->y;

			put_pixel_to_image(app->image, x, y, get_pixel_color(app->sprite, texture_coord.x, texture_coord.y));
			put_pixel_to_image(app->image, x, (abs)(y - WIN_H)  , get_pixel_color(app->sprite, texture_coord.x + (((int)floor_pos->x + (int)floor_pos->y) % 8) * 64, texture_coord.y));

		}
}

static void	floor_cast(t_app *app, int y, t_vector2 *step, t_vector2 *floor_pos)
{
	t_vector2	ray_left;
	t_vector2	ray_right;
	int			ray_pos;
	double		player_height;
	double		distance;

	ray_left = (t_vector2){app->player.direction.x - app->player.camera_plane.x, app->player.direction.y - app->player.camera_plane.y};
	ray_right = (t_vector2){app->player.direction.x + app->player.camera_plane.x, app->player.direction.y + app->player.camera_plane.y};
	ray_pos = y - WIN_H / 2;
	player_height = 0.5 * WIN_H;

	distance = player_height / ray_pos;
	step->x = distance * (ray_right.x - ray_left.x) / WIN_W;
	step->y = distance * (ray_right.y - ray_left.y) / WIN_W;
	floor_pos->x = app->player.position.x + distance * ray_left.x;
	floor_pos->y = app->player.position.y + distance * ray_left.y;
}

/*
 * Simply render all the ground and floor
*/
void	render_background(t_app *app)
{
	int				y;
	t_vector2		step;
	t_vector2		floor_pos;

	y = WIN_H / 2;
	while (y < WIN_H)
	{
		floor_cast(app, y, &step, &floor_pos);
		draw_horizontal_line(app, y, &step, &floor_pos);
		y++;
	}
}

/*
 * Renders the current view of the player.
*/
void	*render_view(void *data)
{
	t_thread_data	*t;
	t_app			*app;
	int				x;
	t_rayhit		rayhit;

	t = (t_thread_data *)data;
	app = (t_app *)t->app;
	x = t->x_start;
	while (x < t->x_end)
	{
		rayhit = raycast(app, x);
		draw_vertical_line(app, x, (int)(WIN_H / rayhit.distance), rayhit);
		x++;
	}
	pthread_exit(NULL);
}


/*
 * Renders the current view of the player with multithreading.
*/
void	render_multithreading(t_app *app)
{
	int			id;
	pthread_t	thread_identifiers[THREADS_MAX];

	id = 0;
	while (id < app->conf->thread_count)
	{
		if (pthread_create(&thread_identifiers[id], NULL, render_view,
				(void *)(&(app->thread_info)[id])))
			exit_error(MSG_ERROR_THREADS);
		id++;
	}
	id = 0;
	while (id < app->conf->thread_count)
	{
		if (pthread_join(thread_identifiers[id], NULL) != 0)
			exit_error(MSG_ERROR_THREADS_JOIN);
		id++;
	}
	cast_objects(app);
	mlx_put_image_to_window(app->mlx, app->win, app->image->img, 0, 0);
}

