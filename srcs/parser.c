/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htahvana <htahvana@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 11:03:15 by saaltone          #+#    #+#             */
/*   Updated: 2022/07/11 15:05:20 by htahvana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static int	map_is_valid(char *data)
{
	int	i;

	if (!data)
		return (0);
	i = 0;
	while (data[i])
	{
		if ((data[i] < '0' || data[i] > '9')
			&& data[i] != ' '
			&& data[i] != '\n')
			return (0);
		i++;
	}
	if (i < MAP_HEIGHT * MAP_WIDTH * 5 - 1)
		return (0);
	return (1);
}

/*
 * Parses the map file and saves it into 3d char array.
*/
int	parse_map(t_app *app)
{
	int		i;
	int		skip;
	int		fd;
	char	buffer[MAP_HEIGHT * MAP_WIDTH * 5 + 1];

	fd = open(MAP_FILE, O_RDONLY);
	if (fd < 0)
		exit_error(MSG_ERROR_MAP_FILE_ACCESS);
	if (read(fd, buffer, MAP_HEIGHT * MAP_WIDTH * 5) < 0 || close(fd) < 0)
		exit_error(MSG_ERROR_MAP_FILE_ACCESS);
	if (!map_is_valid((char *)&buffer))
		exit_error(MSG_ERROR_MAP_INVALID);
	i = 0;
	skip = 0;
	while (buffer[i + skip])
	{
		if (buffer[i + skip] == ' ' || buffer[i + skip] == '\n')
			skip++;
		else
		{
			app->map[i / MAP_HEIGHT][i % MAP_WIDTH][0] = buffer[i + skip];
			app->map[i / MAP_HEIGHT][i % MAP_WIDTH][1] = buffer[i + ++skip];
			app->map[i / MAP_HEIGHT][i % MAP_WIDTH][2] = buffer[i + ++skip];
			app->map[i / MAP_HEIGHT][i % MAP_WIDTH][3] = buffer[i + ++skip];
			i++;
		}
	}
	return (1);
}
