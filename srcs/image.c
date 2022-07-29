/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htahvana <htahvana@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 15:34:30 by saaltone          #+#    #+#             */
/*   Updated: 2022/07/29 17:31:21 by htahvana         ###   ########.fr       */
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

/* void	put_pixel_to_image_depth(t_image *image, int x, int y, unsigned int color)
{
	int		pixel_pos;
	char	*pixel;
	unsigned int	depth;

	pixel_pos = (y * image->line_size) + (x * IMAGE_PIXEL_BYTES);
	if (pixel_pos < 0 || x >= image->width || y >= image->height)
		return ;
	pixel = image->data + pixel_pos;
	depth = *(int *)pixel >> 24;
	if(DEPTH)
		color = ((color >> 8) & 0xFF0000) | (color & 0xFF000000); 
	color = add_shade(1.f - ((color & 0xFF000000) >> 24) / 255.f , color);
	if(depth == 0 || !(depth <= (color >> 24)))
		*(int *)pixel = color;
} */

void	put_pixel_to_image_depth(t_image *image, t_image *depth_image, int x, int y, int color, float distance)
{
	int		pixel_pos;
	char	*pixel;
	char	*depth_pixel;

	pixel_pos = (y * image->line_size) + (x * IMAGE_PIXEL_BYTES);
	if (pixel_pos < 0 || x >= image->width || y >= image->height)
		return ;
	pixel = image->data + pixel_pos;
	depth_pixel = depth_image->data + pixel_pos;
	if(*(float*)depth_pixel == 0.f || *(float*)depth_pixel > distance)
	{
		//color = add_shade(0.9f - distance / MAX_RAY_DISTANCE, color);
		(void)color;
		*(float *)depth_pixel = distance;
		if(color > 0)
		*(int *)pixel = color;
	}
}

void read_depthmap(t_image *depth_image)
{
	int		pixel_pos;
	char	*pixel;
	int i = -1;
	int pixels;

	pixel_pos = 0;
	pixels = WIN_W * WIN_H;
	while (++i < pixels - 1)
	{
		pixel = depth_image->data + pixel_pos;
		*(int *)pixel = (255 - ((int)(254 / MAX_RAY_DISTANCE * (*(float *)pixel)))) << 24;
		pixel_pos += IMAGE_PIXEL_BYTES;
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
	if (pixel_pos < 0 || x > image->width || y > image->height)
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
