/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 15:18:47 by saaltone          #+#    #+#             */
/*   Updated: 2022/08/08 17:21:34 by saaltone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/**
 * Exits the program and prints an error message to stderr (either with perror
 * or from variable).
 */
void	exit_error(char *message)
{
	if (message)
		ft_putendl_fd(message, 1);
	else
		perror(MSG_ERROR);
	exit(EXIT_FAILURE);
}
