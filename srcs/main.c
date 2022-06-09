/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 14:21:47 by saaltone          #+#    #+#             */
/*   Updated: 2022/06/09 11:20:05 by saaltone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

int	main(void)
{
	t_app	*app;

	if (!app_init(&app) || !parse_map(app) || !conf_init(app))
		exit_error(NULL);
	app_run(app);
	return (0);
}
