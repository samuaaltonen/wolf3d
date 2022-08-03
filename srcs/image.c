/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htahvana <htahvana@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 15:34:30 by saaltone          #+#    #+#             */
/*   Updated: 2022/08/03 11:38:26 by htahvana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
 * Initializes image.
*/
t_image	*init_image(void *mlx, int width, int height)
{
	t_image	*image;

	image = (t_image *)malloc(sizeof(t_image));
	if (!image)
		return (NULL);
	image->width = width;
	image->height = height;
	image->img = mlx_new_image(mlx, image->width, image->height);
	image->data = mlx_get_data_addr(image->img, &(image->bpp),
			&(image->line_size), &(image->endian));
	return (image);
}

/*
 * Initializes image.
*/
t_image	*init_xpm_image(void *mlx, int width, int height, char *path)
{
	t_image	*image;

	image = (t_image *)malloc(sizeof(t_image));
	if (!image)
		return (NULL);
	image->width = width;
	image->height = height;
	image->img = mlx_xpm_file_to_image(mlx, path, &(image->width), &(image->height));
	if(image->img == NULL)
		exit_error("image failed to load\n");
	image->data = mlx_get_data_addr(image->img, &(image->bpp),
			&(image->line_size), &(image->endian));
	return (image);
}

/* 
 * Changes color of specific pixel in image.
 *
 * line_size = one line/row size in BYTES in image
 * For example: 1920 window width, and 4 bytes per pixel (32 bits, this info is 
 * 				stored in bpp = bits per pixel), line_size would be 7680)
 * 
 * Change pixel by given x,y coordinates: 	
 * 		Count the correct position in image string by
 * 			 y * line_size * 4 + x * 4 (or bpp / 8)
 * 
 * To change color, we need to cast pixel pointer to int pointer type
*/
void	put_pixel_to_image(t_image *image, int x, int y, int color)
{
	int		pixel_pos;
	char	*pixel;

	pixel_pos = (y * image->line_size) + (x * IMAGE_PIXEL_BYTES);
	if (pixel_pos < 0 || x >= image->width || y >= image->height)
		return ;
	pixel = image->data + pixel_pos;
	*(int *)pixel = color;
}

void	put_pixel_to_image_depth(t_image *image, t_image *depth_image, int x, int y, int color, float distance)
{
	int		pixel_pos;
	char	*pixel;
	char	*depth_pixel;
	unsigned int		depth;

	pixel_pos = (y * image->line_size) + (x * IMAGE_PIXEL_BYTES);
	if (pixel_pos < 0 || x >= image->width || y >= image->height)
		return ;
	pixel = image->data + pixel_pos;
	depth_pixel = depth_image->data + pixel_pos;
	depth = (255 - ((unsigned int)(254 / MAX_RAY_DISTANCE * distance)));
	*(unsigned int *)depth_pixel = depth << 24;
	if(color > 0)
		*(unsigned int *)pixel = color;
}

void	put_pixel_to_image_test(t_image *image, t_image *depth_image, int x, int y, int color, float distance)
{
	int		pixel_pos;
	char	*pixel;
	char	*depth_pixel;
	unsigned int		depth;

	pixel_pos = (y * image->line_size) + (x * IMAGE_PIXEL_BYTES);
	if (pixel_pos < 0 || x >= image->width || y >= image->height)
		return ;
	pixel = image->data + pixel_pos;
	depth_pixel = depth_image->data + pixel_pos;
	(void)depth;
	depth = (255 - ((unsigned int)(254 / MAX_RAY_DISTANCE * distance)));

	if(*(unsigned int*)depth_pixel == 0 || *(unsigned int*)depth_pixel < depth << 24 )
	{
		*(unsigned int *)depth_pixel = depth << 24;
		if(color > 0)
		*(unsigned int *)pixel = color;
	}
}


static void make_bloom(t_image *depthmap, t_point *coord)
{
	t_point cur;
	int size;
	int effect;
	int pixel_pos;
	unsigned char	*pixel;
	unsigned char	*bluepixel;

	size = 15;
	effect = 1;
	cur.x = (coord->x - size - 1);
	cur.y = (coord->y - size - 1);
	while(++cur.y <= coord->y + size)
	{
		if (cur.y < 0)
			cur.y = 0;
		if (cur.y == depthmap->height)
			break;
		cur.x = coord->x - size - 1;
		while(++cur.x < coord->x + size)
		{
			if(cur.x < 0)
				cur.x = 0;
			if(cur.x >= depthmap->width)
				break;
			pixel_pos = (cur.y * depthmap->line_size) + (cur.x * IMAGE_PIXEL_BYTES) + 3;
			pixel = (unsigned char*)(depthmap->data + pixel_pos);
			bluepixel = (unsigned char*)(depthmap->data + pixel_pos - 3);
			if((*( unsigned char *)bluepixel) + effect < 254)
				*( unsigned char *)bluepixel = ((*(unsigned char *)bluepixel) + effect);
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
	coord.x = ((t_thread_data *)data)->id;
	while (coord.x <= WIN_W)
	{
		coord.y = 0;
		while (coord.y < WIN_H - 1)
		{
			pixel_pos = (coord.y * app->depthmap->line_size) + (coord.x * IMAGE_PIXEL_BYTES);
			coord.y = coord.y + 2;
			if ((*(int*)(app->depthmap->data + pixel_pos) & 0xFF000000) > 0xA0000000)
				continue;
			else if (*(int*)(app->image->data + pixel_pos) & 0xFF000000) 
				make_bloom(app->depthmap, &coord);
		}
		coord.x = coord.x + app->conf->thread_count + 2;
	}
	pthread_exit(NULL);
}

void *read_bloom(void *data)
{
	t_app			*app;
	int		pixel_pos;
	char	*depth_pixel;
	char	*pixel;
	int i;
	int pixels;
	int value;

	app = (t_app *)((t_thread_data *)data)->app;
	pixel_pos = -(IMAGE_PIXEL_BYTES * ((t_thread_data *)data)->id);
	pixels = WIN_W * WIN_H;
	i = -((t_thread_data *)data)->id;
	while (i < pixels)
	{
		i += app->conf->thread_count;
		if (i < 0 || i >= pixels)
			continue;
		pixel_pos += IMAGE_PIXEL_BYTES * app->conf->thread_count;
		depth_pixel = app->depthmap->data + pixel_pos;
		pixel = app->image->data + pixel_pos;
		if(*(int *)pixel & 0xFF000000)
		{
			*(int *)depth_pixel = 0xFF000000;
			continue;
		}
		value = ((*(int *)depth_pixel & 0xFF000000) >> 24) + ((*(int *)depth_pixel & 255));
		if(value > 255)
			*(int *)depth_pixel = 0xFF000000;
		else
			*(int *)depth_pixel = value << 24;
	}
	pthread_exit(NULL);
}

/*
 * Returns pixel color at given position.
*/
int	get_pixel_color(t_image *image, int x, int y)
{
	int		pixel_pos;
	char	*pixel;

	pixel_pos = (y * image->line_size) + (x * IMAGE_PIXEL_BYTES);
	if (pixel_pos < 0 || x >= image->width || y >= image->height)
		return (0);
	pixel = image->data + pixel_pos;
	return (*(int *)pixel);
}

/*
 * Flushes image (sets all pixels to black)
*/
void	flush_image(t_image *image)
{
	ft_bzero(image->data, image->height * image->line_size);
}
