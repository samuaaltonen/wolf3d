/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 14:21:47 by saaltone          #+#    #+#             */
/*   Updated: 2022/08/08 17:05:08 by saaltone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

int	main(int argc, char **argv)
{
	t_app	*app;
	char	*path;

	if (argc > 2)
		exit_error(MSG_ERROR_TOO_MANY_ARGUMENTS);
	if (argc == 2)
		path = ft_strdup(argv[1]);
	else
		path = ft_strdup(MAP_FILE_DEFAULT);
	if (!path)
		exit_error(NULL);
	app = NULL;
	if (!path
		|| !app_init(&app, path)
		|| !parse_map(app, path)
		|| !conf_init(app))
		exit_error(NULL);
	free(path);
	app_prepare(app);
	mlx_loop(app->mlx);
	return (0);
}
