/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htahvana <htahvana@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 14:32:45 by saaltone          #+#    #+#             */
/*   Updated: 2022/07/25 13:17:32 by htahvana         ###   ########.fr       */
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
	start_pixel = WIN_H / 2 - height / 2 - 1;
	end_pixel = WIN_H / 2 + height / 2;
	if (start_pixel < 0)
		start_pixel = 0;
	if (end_pixel >= WIN_H)
		end_pixel = WIN_H - 1;
	i = 0;
	if(rayhit.distance > MAX_RAY_DISTANCE)
		rayhit.distance = MAX_RAY_DISTANCE;
	rayhit.distance = 254 / MAX_RAY_DISTANCE * rayhit.distance + 1.f;
	while (i < height + 1)
	{
		tex_y += y_step;
		if(1) //toggle for cardinal texturing
			put_pixel_to_image(app->image, x, start_pixel + i, get_pixel_color(app->sprite, rayhit.tex_x + (rayhit.type - 'A') * 64, (int)tex_y & (TEX_SIZE - 1)) | ((int)rayhit.distance << 24));
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
	double		distance;

	distance = 0.5 * WIN_H / (y - WIN_H / 2);
	x = -1;
	if(distance > MAX_RAY_DISTANCE)
			distance = MAX_RAY_DISTANCE;
		distance = 254 / MAX_RAY_DISTANCE * distance + 1;

	while (++x < WIN_W)
	{
		coord = (t_point){(int)floor_pos->x, (int)floor_pos->y};
		texture_coord.x = (int)(TEX_SIZE * (floor_pos->x - coord.x)) & (TEX_SIZE - 1);
		texture_coord.y = (int)(TEX_SIZE * (floor_pos->y - coord.y)) & (TEX_SIZE - 1);
		floor_pos->x += step->x;
		floor_pos->y += step->y;
		if(!check_ray_pos(app, floor_pos))
			continue;
		put_pixel_to_image(app->image, x, y, get_pixel_color(app->sprite, texture_coord.x + (app->map[(int)floor_pos->y][(int)floor_pos->x][1] - 'A') * TEX_SIZE , texture_coord.y) | ((int)distance << 24));
		put_pixel_to_image(app->image, x, (abs)(y - WIN_H) - 1, get_pixel_color(app->sprite, texture_coord.x + (app->map[(int)floor_pos->y][(int)floor_pos->x][2] - 'A') * TEX_SIZE, texture_coord.y) | ((int)distance << 24));
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
	ray_pos = y - WIN_H * 0.5f;
	player_height = 0.5f * WIN_H;

	distance = player_height / ray_pos;
	step->x = distance * (ray_right.x - ray_left.x) / (double)WIN_W;
	step->y = distance * (ray_right.y - ray_left.y) / (double)WIN_W;
	floor_pos->x = app->player.position.x + distance * ray_left.x;
	floor_pos->y = app->player.position.y + distance * ray_left.y;
}

/*
 * Simply render all the ground and floor
*/
void	*render_background(void *data)
{
	t_thread_data	*t;
	t_app			*app;
	int				y;
	t_vector2		step;
	t_vector2		floor_pos;

	t = (t_thread_data *)data;
	app = (t_app *)t->app;
	y = t->y_start - 1;
	while (++y <= t->y_end)
	{
		floor_cast(app, y, &step, &floor_pos);
		draw_horizontal_line(app, y, &step, &floor_pos);
	}
	pthread_exit(NULL);
}

/* 
 * Object multithreaded rendering
 */
void	*render_objects(void *data)
{
	t_thread_data	*t;
	t_app			*app;
	int				i;
	t_vector2		dist;
	double			distance;
	t_vector2		transform;
	int				screen_x;

	t = (t_thread_data *)data;
	app = (t_app *)t->app;
	i = t->id - 1;
	while (++i <= (t->id + 1) * app->objects_pool_size)
	{
		if(i >= app->object_count || app->objects[i].active == 0)
			continue;
		dist.x = (app->objects[i].position.x - app->player.position.x) * app->object_sprites[app->objects[i].sprite_id].offset_multiplier;
		dist.y = (app->objects[i].position.y - app->player.position.y) * app->object_sprites[app->objects[i].sprite_id].offset_multiplier;
		transform = ft_vector_multiply_matrix(dist, ft_matrix_inverse((t_matrix2){
			app->player.camera_plane,
			app->player.direction
		}));
		distance = ft_vector_length(dist);

		if ((transform.y / distance < 0.75f))
			continue;

		double rad = atan2(dist.x, dist.y);
		if(rad < 0)
			rad = rad + 2 * M_PI;
		rad = rad * (180 / M_PI);
		if(app->object_sprites[app->objects[i].sprite_id].mirrored)
			app->objects[i].frame_id = ((int)(rad * 64 / 180) % 64);
		else
			app->objects[i].frame_id = ((int)(rad * 64 / 360) % 64);
		if(app->objects[i].sprite_id < 2)
			app->objects[i].frame_id = app->object_sprites[app->objects[i].sprite_id].animation_step;
		//ft_printf("%i\n", ((app->objects[i].frame_id)));
		if(distance > MAX_RAY_DISTANCE)
			distance = MAX_RAY_DISTANCE;
		screen_x = (int)((WIN_W / 2) * (1.0f + (transform.x / transform.y)));

		app->objects[i].width = abs((int)(WIN_H / transform.y));
		app->objects[i].height = abs((int)(WIN_H / transform.y));
		draw_object(app, i, screen_x, (int)(254 / MAX_RAY_DISTANCE * distance + 1.f));
		}
	pthread_exit(NULL);
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
	x = t->x_start - 1;
	while (++x <= t->x_end)
	{
		if(!raycast(app, x, &rayhit))
			continue;
		draw_vertical_line(app, x, (int)(WIN_H / rayhit.distance), rayhit);
		//app->distance_buffer[x] = rayhit.distance;
	}
	pthread_exit(NULL);
}

/*
 * Renders the current view of the player with multithreading.
*/
void	render_multithreading(t_app *app, void *(*renderer)(void *))
{
	int			id;
	pthread_t	thread_identifiers[THREADS_MAX];

	id = 0;
	while (id < app->conf->thread_count)
	{
		if (pthread_create(&thread_identifiers[id], NULL, renderer,
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
}