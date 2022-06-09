/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_mouse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 15:40:40 by saaltone          #+#    #+#             */
/*   Updated: 2022/06/08 13:55:38 by saaltone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
 * Handles events for mouse..
*/
int	events_mouse(int mousecode, int x, int y, t_app *app)
{
	(void) mousecode;
	(void) x;
	(void) y;
	(void) app;
	return (0);
}

/*
 * Tracks mouse position and sets initial c value depending on mouse
 * coordinates. (This is used in Julia set).
*/
int	events_mouse_track(int x, int y, t_app *app)
{
	(void) x;
	(void) y;
	(void) app;
	return (0);
}