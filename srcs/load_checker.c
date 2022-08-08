/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_checker.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 14:36:48 by htahvana          #+#    #+#             */
/*   Updated: 2022/08/08 17:22:49 by saaltone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/**
 * Checks if the textures are the correct size
 */
int	check_textures(t_app *app)
{
	int		i;
	t_image	*temp;

	temp = app->sprite;
	if (temp->height != TEX_SIZE
		|| temp->width != TEX_SIZE * TEX_COUNT)
		return (FALSE);
	i = -1;
	while (++i < MAP_MAX_OBJECT_IDS)
	{
		temp = app->object_sprites[i].image;
		if (temp->height != TEX_SIZE
			|| temp->width != TEX_SIZE * 64)
			return (FALSE);
	}
	temp = app->bg;
	if (temp->height != 128
		|| temp->width != 512)
		return (FALSE);
	return (TRUE);
}
