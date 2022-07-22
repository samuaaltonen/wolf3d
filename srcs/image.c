/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htahvana <htahvana@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 15:34:30 by saaltone          #+#    #+#             */
/*   Updated: 2022/07/22 17:54:22 by htahvana         ###   ########.fr       */
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

static int	add_shade(double shade, int color)
{
        return ((((color & 0xFF000000))) |
				(int)(((color & 0xFF0000) >> 16) * shade) << 16 |
            	(int)(((color & 0x00FF00) >> 8) * shade) << 8 |
                (int)(((color & 0x0000FF)) * shade));
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
	if(DEPTH)
		color = ((color >> 8) & 0xFF0000) | (color & 0xFF000000); 
	color = add_shade(1.f - ((color & 0xFF000000) >> 24) / 255.f , color);
	*(int *)pixel = color;
}

void	put_pixel_to_image_depth(t_image *image, int x, int y, unsigned int color)
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
