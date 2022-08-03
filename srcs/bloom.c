/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bloom.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htahvana <htahvana@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 14:18:55 by htahvana          #+#    #+#             */
/*   Updated: 2022/08/03 14:26:34 by htahvana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

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