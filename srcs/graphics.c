/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htahvana <htahvana@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 14:32:45 by saaltone          #+#    #+#             */
/*   Updated: 2022/08/03 11:56:38 by htahvana         ###   ########.fr       */
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

	height += 2;
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
	clamp_distance(&rayhit.distance);
	while (i < height)
	{
		tex_y += y_step;
		if(rayhit.type) //toggle for cardinal texturing
			put_pixel_to_image_depth(app->image, app->depthmap, x, start_pixel + i, get_pixel_color(app->sprite, rayhit.tex_x + (rayhit.type - 'A') * 64, (int)tex_y & (TEX_SIZE - 1)), rayhit.distance);
		else
			put_pixel_to_image_depth(app->image, app->depthmap, x, start_pixel + i, 0, rayhit.distance);

		i++;
	}
}

void	clamp_distance(double *distance)
{
	
	if(*distance > MAX_RAY_DISTANCE)
		*distance = MAX_RAY_DISTANCE;
		//*distance = 254 / MAX_RAY_DISTANCE * *distance + 1;
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

	distance = 0.5f * WIN_H / (y - WIN_H / 2);
	x = -1;
	clamp_distance(&distance);
	while (++x < WIN_W)
	{
		coord = (t_point){(int)floor_pos->x, (int)floor_pos->y};
		texture_coord.x = (int)(TEX_SIZE * (floor_pos->x - coord.x)) & (TEX_SIZE - 1);
		texture_coord.y = (int)(TEX_SIZE * (floor_pos->y - coord.y)) & (TEX_SIZE - 1);
		if(!check_ray_pos(app, floor_pos))
		{
					floor_pos->x += step->x;
		floor_pos->y += step->y;
					continue;
		}
		if(app->map[(int)floor_pos->y][(int)floor_pos->x][1] - 'A' >= DOOR_MAP_ID)
				put_pixel_to_image_depth(app->image, app->depthmap, x, y, 0, distance);
		else
			put_pixel_to_image_depth(app->image, app->depthmap, x, y, get_pixel_color(app->sprite, texture_coord.x + (app->map[(int)floor_pos->y][(int)floor_pos->x][1] - 'A') * TEX_SIZE , texture_coord.y), distance);
		if(app->map[(int)floor_pos->y][(int)floor_pos->x][2] - 'A' >= DOOR_MAP_ID)
			put_pixel_to_image_depth(app->image, app->depthmap, x, (abs)(y - WIN_H) - 1, 0, distance);
		else
			put_pixel_to_image_depth(app->image, app->depthmap, x, (abs)(y - WIN_H) - 1, get_pixel_color(app->sprite, texture_coord.x + (app->map[(int)floor_pos->y][(int)floor_pos->x][2] - 'A') * TEX_SIZE, texture_coord.y), distance);
		floor_pos->x += step->x;
		floor_pos->y += step->y;
	}
}

static void	floor_cast(t_app *app, int y, t_vector2 *step, t_vector2 *floor_pos)
{
	t_vector2	ray_left;
	t_vector2	ray_right;
	int			ray_pos;
	double		player_height;
	double		distance;

	ray_left = (t_vector2){app->player.dir.x - app->player.cam.x, app->player.dir.y - app->player.cam.y};
	ray_right = (t_vector2){app->player.dir.x + app->player.cam.x, app->player.dir.y + app->player.cam.y};
	ray_pos = y - WIN_H * 0.5f;
	player_height = 0.5f * WIN_H;

	distance = player_height / ray_pos;
	step->x = distance * (ray_right.x - ray_left.x) / (double)WIN_W;
	step->y = distance * (ray_right.y - ray_left.y) / (double)WIN_W;
	floor_pos->x = app->player.pos.x + distance * ray_left.x;
	floor_pos->y = app->player.pos.y + distance * ray_left.y;
	//ft_printf("x%f y%f\n", floor_pos->x, floor_pos->y);
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
	floor_pos.x = 0.f;
	floor_pos.y = 0.f;
	while (++y <= t->y_end)
	{
		floor_cast(app, y, &step, &floor_pos);
		draw_horizontal_line(app, y, &step, &floor_pos);
	}
	pthread_exit(NULL);
}


static double	get_radial_direction(t_vector2 *vector)
{
	double	rad;

	rad = atan2(vector->x, vector->y);
	if(rad < 0)
		rad = rad + 2 * M_PI;
	rad = rad * (180 / M_PI);
	return (rad);
}


/*
 * Renders the skybox.
*/
void	*render_skybox(void *data)
{
	t_app			*app;
	t_point			coord;
	t_vector2		steps;
	int offset;
	double texy;

	app = (t_app *)((t_thread_data *)data)->app;
	coord.x = ((t_thread_data *)data)->x_start - 1;
	steps.y = 128 / (double)WIN_H;
	steps.x = 512 / (double)WIN_W / 2;
	while (++coord.x <= ((t_thread_data *)data)->x_end)
	{
		coord.y = 0;
		texy = 0.f;
		offset = (int)((coord.x + app->conf->skybox_offset / 720.f * WIN_W * 2) * steps.x) % 512;
		while (++coord.y < WIN_H - 1)
		{
			put_pixel_to_image(app->image, coord.x, coord.y, get_pixel_color(app->bg, offset, texy) | 16777216);
			texy += steps.y;
		}
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
	float			distance;
	t_vector2		transform;
	int				screen_x;
	double			rad;

	t = (t_thread_data *)data;
	app = (t_app *)t->app;
	i = -(t->id);
	while (i < app->object_count)
	{
		i += app->conf->thread_count;
		if (i < 0 || i >= app->object_count || app->objects[i].active == 0)
			continue;
		dist.x = (app->objects[i].position.x - app->player.pos.x) * app->object_sprites[app->objects[i].sprite_id].offset_multiplier;
		dist.y = (app->objects[i].position.y - app->player.pos.y) * app->object_sprites[app->objects[i].sprite_id].offset_multiplier;
		transform = ft_vector_multiply_matrix(dist, ft_matrix_inverse((t_matrix2){
			app->player.cam,
			app->player.dir
		}));
		distance = ft_vector_length(dist);
		if ((transform.y / distance < 0.75f))
			continue;
		rad = get_radial_direction(&dist);
		if (app->object_sprites[app->objects[i].sprite_id].mirrored)
			app->objects[i].frame_id = ((int)(rad * 64 / 180) % 64);
		else
			app->objects[i].frame_id = ((int)(rad * 64 / 360) % 64);
		if (app->objects[i].sprite_id < 2)
			app->objects[i].frame_id = app->object_sprites[app->objects[i].sprite_id].animation_step;
		screen_x = (int)((WIN_W / 2) * (1.0f + (transform.x / transform.y)));
		app->objects[i].width = abs((int)(WIN_H / transform.y));
		app->objects[i].height = abs((int)(WIN_H / transform.y));
		clamp_distance(&transform.y);
		app->objects[i].distance = transform.y;
		draw_object(app, &app->objects[i], screen_x);
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
		raycast(app, x, &rayhit);
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