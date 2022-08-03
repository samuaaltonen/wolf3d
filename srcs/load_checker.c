/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_checker.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htahvana <htahvana@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 14:36:48 by htahvana          #+#    #+#             */
/*   Updated: 2022/08/03 15:05:08 by htahvana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

int		check_textures(t_app *app)
{
	int		i;
	t_image	*temp;

	temp = app->sprite;
	if (temp->height != TEX_SIZE
			|| temp->width != TEX_SIZE * TEX_COUNT)
		return (0);
	i = -1;
	while (++i < MAP_MAX_OBJECT_IDS)
	{
		temp = app->object_sprites[i].image;
		if (temp->height != TEX_SIZE
				|| temp->width != TEX_SIZE * 64)
			return (0);
	}
	temp = app->bg;
	if (temp->height != 128
			|| temp->width != 512)
		return (0);
	return (1);
}