/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_loop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 15:41:22 by saaltone          #+#    #+#             */
/*   Updated: 2022/06/08 18:37:10 by saaltone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
 * Handles continuous events.
*/
int	events_loop(t_app *app)
{
	app_render(app);
	return (0);
}
