/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 14:21:47 by saaltone          #+#    #+#             */
/*   Updated: 2022/08/05 12:15:39 by saaltone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

int	main(int argc, char **argv)
{
	t_app	*app;
	char	*path;
	int		fd;

	fd = -1;
	app = NULL;
	if (argc == 2)
	{
		path = ft_strnew(ft_strlen(argv[1]));
		if (!path)
			exit_error(NULL);
		ft_strcpy(path, argv[1]);
		fd = open(path, O_RDONLY);
		if (fd < 0 || !app_init(&app, path) || !parse_map(app, path)
			||!conf_init(app))
			exit_error(NULL);
	}
	else if (!app_init(&app, MAP_FILE) || !parse_map(app, MAP_FILE)
		|| !conf_init(app))
		exit_error(NULL);
	app_run(app);
	mlx_loop(app->mlx);
	return (0);
}
