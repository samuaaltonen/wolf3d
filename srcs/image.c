/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htahvana <htahvana@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 15:34:30 by saaltone          #+#    #+#             */
/*   Updated: 2022/08/03 15:32:20 by htahvana         ###   ########.fr       */
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
	image->img = mlx_xpm_file_to_image(mlx, path, &(image->width),
		&(image->height));
	if (image->img == NULL)
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

/*
 * put pixel to image and depthmap
 */
void	put_pixel_to_image_depth(t_app *app, t_point point, int color,
	float distance)
{
	int				pixel_pos;
	char			*pixel;
	char			*depth_pixel;
	unsigned int	depth;

	pixel_pos = (point.y * app->image->line_size)
		+ (point.x * IMAGE_PIXEL_BYTES);
	if (pixel_pos < 0 || point.x >=  app->image->width
			|| point.y >= app->image->height)
		return ;
	pixel = app->image->data + pixel_pos;
	depth_pixel = app->depthmap->data + pixel_pos;
	depth = (255 - ((unsigned int)(254 / MAX_RAY_DISTANCE * distance)));
	*(unsigned int *)depth_pixel = depth << 24;
	if (color > 0)
		*(unsigned int *)pixel = color;
}

/*
 * tests if there is something over the pixel from depthmap before putting it.
 */
void	put_pixel_to_image_test(t_app *app, t_point point, int color,
	float distance)
{
	int				pixel_pos;
	char			*pixel;
	char			*depth_pixel;
	unsigned int	depth;

	pixel_pos = (point.y * app->image->line_size)
		+ (point.x * IMAGE_PIXEL_BYTES);
	if (pixel_pos < 0 || point.x >= app->image->width
			|| point.y >= app->image->height)
		return ;
	pixel = app->image->data + pixel_pos;
	depth_pixel = app->depthmap->data + pixel_pos;
	depth = (255 - ((unsigned int)(254 / MAX_RAY_DISTANCE * distance)));
	if (*(unsigned int*)depth_pixel == 0
		|| *(unsigned int*)depth_pixel < depth << 24 )
	{
		*(unsigned int *)depth_pixel = depth << 24;
		if (color > 0)
			*(unsigned int *)pixel = color;
	}
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
