/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htahvana <htahvana@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 15:36:58 by htahvana          #+#    #+#             */
/*   Updated: 2022/07/07 17:59:35 by htahvana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

t_image	*init_custom_image(void *mlx, int width, int height);


static char	*read_image(t_app *app)
{
	void	*img;
	void	*relative_path = "./wolftextures.xpm";
	int		img_width;
	int		img_height;
	int	bpp = 32;
	int linesize = 512;
	int endian = 0;

	img = mlx_xpm_file_to_image(app->mlx, relative_path, &img_width, &img_height);
	char *test = mlx_get_data_addr(img, &(bpp),
			&(linesize), &(endian));
	if(img == NULL)
		exit_error("image failed to load\n");
	return (test);
}

static t_image	*load_image(int id, char *img, t_app *app)
{	
	int	test_x;
	int test_y;

	(void)id;
	(void)img;
	t_image *sprite = init_custom_image(app->mlx, 64, 64);
	test_y = -1;
	while (++test_y < 64)
	{
		test_x = -1;
		while (++test_x < 64)
			put_pixel_to_image(sprite, test_x, test_y, *((int *)(img + ((test_y * 512 * 4) + (id * 64 * 4 + test_x * 4)))));
				
	}
	return (sprite);
}

/* 	bytes = image->bpp / 8;
	pixel_pos = (y * image->line_size) + (x * bytes);
	if (pixel_pos < 0 || x > image->width || y > image->height)
		return (0);
	pixel = image->data + pixel_pos; */

int	mouse_activity_track(int x, int y, t_app *app)
{
	(void)x;
	(void)y;
	(void)app;
 	/* int sprite_x;
	int sprite_y; */
	t_image *sprite = NULL;
	char *img = NULL;
	
	if(img == NULL)
	{
		img = read_image(app);
		sprite = load_image(2, (char *)img, app);
	}
	mlx_put_image_to_window(app->mlx, app->win, sprite->img, x, y);
/* 	sprite_y = -1;
	while(++sprite_y < 64)
	{
		sprite_x = -1; 
		while(sprite_x < 64)
		{
			sprite_x++;
			put_pixel_to_image(app->image, sprite_x + x, sprite_y + y,  *((char *)sprite + sprite_y * 64 + sprite_x));

		}
	} */
	return (0);
}