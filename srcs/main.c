/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htahvana <htahvana@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 14:21:47 by saaltone          #+#    #+#             */
/*   Updated: 2022/08/03 15:00:00 by htahvana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

int	main(void)
{
	t_app	*app;

	if (!app_init(&app) || !parse_map(app) || !conf_init(app))
		exit_error(NULL);
	app_run(app);
	if (!check_textures(app))
		exit_error(MSG_ERROR_TEXTURE_LOAD_FAILED);
	mlx_loop(app->mlx);
	return (0);
}
