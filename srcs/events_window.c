/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_window.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 14:13:33 by saaltone          #+#    #+#             */
/*   Updated: 2022/08/01 13:10:05 by saaltone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/**
 * Exits the application when window is destroyed.
*/
int	events_window_destroy(void)
{
	exit(EXIT_SUCCESS);
	return (0);
}
