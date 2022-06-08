/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_key.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 14:15:51 by saaltone          #+#    #+#             */
/*   Updated: 2022/06/08 13:54:09 by saaltone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
 * Handles events for key presses (keyup).
*/
int	events_keyup(int keycode, t_app *app)
{
	if (keycode == KEY_ESC)
		exit(EXIT_SUCCESS);
	if (keycode == KEY_H)
		app->conf->toggle_help = ft_toggle(app->conf->toggle_help);
	return (0);
}

/*
 * Handles events for key presses (keydown).
*/
int	events_keydown(int keycode, t_app *app)
{
	(void) keycode;
	(void) app;
	return (0);
}
