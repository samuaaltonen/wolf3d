/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 14:32:45 by saaltone          #+#    #+#             */
/*   Updated: 2022/06/09 13:22:38 by saaltone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
 * Draws vertical line to image based on wall distance (closer = higher)
*/
static void	draw_vertical_line(t_app *app, int x, int height, int texture)
{
	int	start_pixel;
	int	end_pixel;
	int	i;

	if (height > WIN_H)
		height = WIN_H;
	start_pixel = WIN_H / 2 - height / 2;
	end_pixel = WIN_H / 2 + height / 2;
	if (start_pixel < 0)
		start_pixel = 0;
	if (end_pixel >= WIN_H)
		end_pixel = WIN_H - 1;
	i = 0;
	while (i < height)
	{
		put_pixel_to_image(app->image, x, start_pixel + i, 0xFFFFFF / texture);
		i++;
	}
}

/*
 * Renders the current view of the player.
*/
void	render_view(t_app *app)
{
	int			x;
	int			texture;
	double		distance;

	x = 0;
	while (x < WIN_W)
	{
		texture = raycast(app, x, &distance);
		draw_vertical_line(app, x, (int)(WIN_H / distance), texture);
		x++;
	}
	mlx_put_image_to_window(app->mlx, app->win, app->image->img, 0, 0);
}
