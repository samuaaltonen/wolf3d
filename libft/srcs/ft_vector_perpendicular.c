/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vector_perpendicular.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 12:50:29 by saaltone          #+#    #+#             */
/*   Updated: 2022/07/08 13:26:48 by saaltone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Returns perpendicular 2d vector of vector a.
*/
t_vector2	ft_vector_perpendicular(t_vector2 a)
{
	if (a.y < 0)
		return ((t_vector2){a.x, -a.y});
	return ((t_vector2){-a.x, a.y});
}
