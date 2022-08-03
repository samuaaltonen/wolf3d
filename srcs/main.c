/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htahvana <htahvana@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 14:21:47 by saaltone          #+#    #+#             */
/*   Updated: 2022/08/03 16:18:47 by htahvana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
 * Displays help menu.
 */
void	help_display(t_app *app)
{
	int					i;
	static const char	*h[] = {
		"[arrow left]       Rotate left" , "[arrow right]      Rotate right",
		"[arrow up]/[w]     Move forward", "[arrow down]/[s]   Move backward",
		"[a]                Move left",    "[d]                Move right",
		"[b]                Toggle bloom",
		"[u]                Decrease FOV", "[i]                Increase FOV",
		"[f]                Open Doors",   "[esc]              Exit", NULL,
	};
	mlx_string_put(app->mlx, app->win, WIN_W / 2 - 360,
		WIN_H / 2 - 230, 0xFFFFFF, "Controls:");
	i = -1;
	while (h[++i])
		mlx_string_put(app->mlx, app->win, WIN_W / 2 - 230,
			WIN_H / 2 - 230 + i * 30, 0xFF55FF, (char *) h[i]);
}

// Updates the info string with given value backwards from given index
static void update_value(t_app *app, int value, int char_index)
{
	int	i;

	i = -1;
	while (++i < 3)
	{
		app->fps_info[char_index - i] = value % 10 + '0';
		value = value / 10;
	}
}

// Updates all the values
void update_info(t_app *app)
{
	update_value(app, app->conf->coin_points, 18);
	update_value(app, app->conf->coin_max, 22);
	update_value(app, app->conf->fps, 30);
}

int	main(void)
{
	t_app	*app;

	if (!app_init(&app) || !parse_map(app) || !conf_init(app))
		exit_error(NULL);
	app_run(app);
	mlx_loop(app->mlx);
	return (0);
}
