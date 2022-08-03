/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bloom.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htahvana <htahvana@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 14:18:55 by htahvana          #+#    #+#             */
/*   Updated: 2022/08/03 16:56:08 by htahvana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
 * Adds blue channel value to depthmap surrounding given pixel
 */
static void	make_bloom(t_image *depthmap, t_point *coord)
{
	t_point			cur;
	int				pixel_pos;
	unsigned char	*pixel;
	unsigned char	*bluepixel;

	cur.y = (coord->y - BLOOM_SIZE - 1);
	while (++cur.y <= coord->y + BLOOM_SIZE)
	{
		if (cur.y < 0 || cur.y >= depthmap->height)
			continue ;
		cur.x = coord->x - BLOOM_SIZE - 1;
		while (++cur.x < coord->x + BLOOM_SIZE)
		{
			if (cur.x < 0 || cur.x >= depthmap->width)
				continue ;
			pixel_pos = (cur.y * depthmap->line_size)
				+ (cur.x * IMAGE_PIXEL_BYTES) + 3;
			pixel = (unsigned char *)(depthmap->data + pixel_pos);
			bluepixel = (unsigned char *)(depthmap->data + pixel_pos - 3);
			if ((*(char *)bluepixel) + 1 < 254)
				*(char *)bluepixel = ((*(char *)bluepixel) + 1);
			pixel_pos += IMAGE_PIXEL_BYTES;
		}
	}
}

/*
 * Finds pixels of skybox in image, calls the make_bloom the pixels
 */
void	*render_bloom(void *data)
{
	t_app			*app;
	t_point			coord;
	int				pixel_pos;

	app = (t_app *)((t_thread_data *)data)->app;
	coord.x = ((t_thread_data *)data)->id - 1;
	while (coord.x <= WIN_W)
	{
		coord.y = 0;
		while (coord.y < WIN_H - 1)
		{
			pixel_pos = (coord.y * app->depthmap->line_size)
				+ (coord.x * IMAGE_PIXEL_BYTES);
			coord.y = coord.y + 2;
			if ((*(int *)(app->depthmap->data + pixel_pos) & 0xFF000000)
					> 0xA0000000)
				continue ;
			else if (*(int *)(app->image->data + pixel_pos) & 0xFF000000)
				make_bloom(app->depthmap, &coord);
		}
		coord.x = coord.x + app->conf->thread_count + 2;
	}
	pthread_exit(NULL);
}

/*
 * Adds the blue channel of the depthmap to the transparency channel
 */
void	*read_bloom(void *data)
{
	t_app	*app;
	t_uint8	*depth_pixel;
	int		pixel_pos;
	int		value;

	app = (t_app *)((t_thread_data *)data)->app;
	pixel_pos = IMAGE_PIXEL_BYTES * (((t_thread_data *)data)->id - 1);
	while (pixel_pos < WIN_W * WIN_H * IMAGE_PIXEL_BYTES - 1)
	{
		depth_pixel = (t_uint8 *)(app->depthmap->data + pixel_pos + 3);
		if (*(int *)(app->image->data + pixel_pos) & 0xFF000000)
		{
			*(char *)depth_pixel = 0xFF;
			pixel_pos += app->conf->thread_count * IMAGE_PIXEL_BYTES;
			continue ;
		}
		value = ((*(t_uint8 *)depth_pixel)) + ((*(t_uint8 *)(depth_pixel - 3)));
		if (value > 255)
			*(char *)depth_pixel = 0xFF;
		else
			*(char *)depth_pixel = value;
		*(char *)(depth_pixel - 3) = 0;
		pixel_pos += app->conf->thread_count * IMAGE_PIXEL_BYTES;
	}
	pthread_exit(NULL);
}

/*
 * Renders the skybox.
*/
void	*render_skybox(void *data)
{
	t_app		*app;
	t_point		coord;
	t_vector2	steps;
	int			offset;
	double		texy;

	app = (t_app *)((t_thread_data *)data)->app;
	coord.x = ((t_thread_data *)data)->x_start - 1;
	steps.y = 128 / (double)WIN_H;
	steps.x = 512 / (double)WIN_W / 2;
	while (++coord.x <= ((t_thread_data *)data)->x_end)
	{
		coord.y = 0;
		texy = 0.f;
		offset = (int)((coord.x + app->conf->skybox_offset / 720.f * WIN_W * 2)
				* steps.x) % 512;
		while (++coord.y < WIN_H - 1)
		{
			put_pixel_to_image(app->image, coord.x, coord.y,
				get_pixel_color(app->bg, offset, texy) | 16777216);
			texy += steps.y;
		}
	}
	pthread_exit(NULL);
}
