/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 15:14:06 by saaltone          #+#    #+#             */
/*   Updated: 2022/06/07 16:28:45 by saaltone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	init_thread_info(t_app *app)
{
	int	i;
	int	width_per_thread;

	i = 0;
	width_per_thread = WIN_W / app->conf->thread_count;
	while (i < app->conf->thread_count && i < THREADS_MAX)
	{
		app->thread_info[i] = (t_thread_data){
			app,
			width_per_thread * i,
			width_per_thread * (i + 1),
		};
		i++;
	}
}

int	conf_init(t_app *app)
{
	if (!app)
		return (0);
	app->conf = (t_conf *)malloc(sizeof(t_conf));
	if (!(app->conf))
		return (0);
	app->conf->win_name = WIN_NAME;
	app->conf->toggle_help = 0;
	app->conf->win_w = WIN_W;
	app->conf->win_h = WIN_H;
	app->conf->thread_count = THREADS_DEFAULT;
	init_thread_info(app);
	return (1);
}