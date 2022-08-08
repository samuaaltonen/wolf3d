/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 15:34:30 by saaltone          #+#    #+#             */
/*   Updated: 2022/08/08 17:22:36 by saaltone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/**
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
	if (!image->img)
		exit_error(MSG_ERROR_IMAGE_INIT);
	image->data = mlx_get_data_addr(image->img, &(image->bpp),
			&(image->line_size), &(image->endian));
	if (!image->data)
		exit_error(MSG_ERROR_IMAGE_INIT);
	return (image);
}

/**
 * Initializes XPM image.
 */
t_image	*init_xpm_image(void *mlx, char *path)
{
	t_image	*image;

	image = (t_image *)malloc(sizeof(t_image));
	if (!image)
		return (NULL);
	image->img = mlx_xpm_file_to_image(mlx, path, &(image->width),
			&(image->height));
	if (!image->img)
		exit_error(MSG_ERROR_IMAGE_INIT);
	image->data = mlx_get_data_addr(image->img, &(image->bpp),
			&(image->line_size), &(image->endian));
	if (!image->data)
		exit_error(MSG_ERROR_IMAGE_INIT);
	return (image);
}

/**
 * Changes color of a specific pixel in image.
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

/**
 * Puts pixel to image and depthmap
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
	if (pixel_pos < 0 || point.x >= app->image->width
		|| point.y >= app->image->height)
		return ;
	pixel = app->image->data + pixel_pos;
	depth_pixel = app->depthmap->data + pixel_pos;
	depth = (255 - ((unsigned int)(254 / MAX_RAY_DISTANCE * distance)));
	*(unsigned int *)depth_pixel = depth << 24;
	if (color > 0)
		*(unsigned int *)pixel = color;
}

/**
 * Tests if there is something over the pixel from depthmap before putting it.
 */
void	put_pixel_to_image_check(t_app *app, t_point point, int color,
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
	if (*(unsigned int *)depth_pixel == 0
		|| *(unsigned int *)depth_pixel < depth << 24)
	{
		*(unsigned int *)depth_pixel = depth << 24;
		if (color > 0)
			*(unsigned int *)pixel = color;
	}
}
