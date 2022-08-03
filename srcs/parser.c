/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 11:03:15 by saaltone          #+#    #+#             */
/*   Updated: 2022/08/03 13:32:52 by saaltone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/**
 * Checks if characters used in map are valid.
*/
static int	map_is_valid(char *data, t_app *app)
{
	int	i;
	int	bytes;

	i = -1;
	bytes = 0;
	while (data[++i])
	{
		if (bytes < MAP_BYTES)
		{
			if (!(data[i] >= 'A' && data[i] <= DOOR_MAP_ID)
				&& data[i] != EMPTY_MAP_ID)
				return (0);
			bytes++;
		}
		else if (bytes >= MAP_BYTES)
		{
			if (!(data[i] == ' ' || data[i] == '\n'))
				return (0);
			bytes = 0;
		}
	}
	if (!(i == app->map_size.y * app->map_size.x * 5 - 1))
		return (0);
	return (1);
}

/**
 * Reads through buffer and saves map characters to map array.
*/
static void	save_map(t_app *app, char *buffer)
{
	int	x;
	int	y;
	int	i;
	int	skip;

	i = 0;
	skip = 0;
	while (buffer[i + skip])
	{
		if (buffer[i + skip] == ' ' || buffer[i + skip] == '\n')
			skip++;
		else
		{
			x = i % app->map_size.x;
			y = i / app->map_size.x;
			app->map[y][x][0] = buffer[i + skip];
			app->map[y][x][1] = buffer[i + skip + 1];
			app->map[y][x][2] = buffer[i + skip + 2];
			app->map[y][x][3] = buffer[i + skip + 3];
			skip += 3;
			i++;
		}
	}
}

/**
 * Checks one line from map file. Exits with error if the line is different
 * in length or if there is a problem with reading.
*/
static int	check_map_line(int fd, int *line_length)
{
	int		read;
	char	*line;

	line = NULL;
	read = ft_get_next_line(fd, &line);
	if (read == 0)
		return (read);
	if (read < 0)
		exit_error(MSG_ERROR_MAP_INVALID);
	if (*line_length == 0)
		*line_length = ft_strlen(line);
	else if (*line_length != (int) ft_strlen(line))
		exit_error(MSG_ERROR_MAP_INVALID);
	if (read > 0)
		free(line);
	return (read);
}

/**
 * Checks map dimensions and shape.
*/
int	check_map(t_app *app)
{
	int		fd;
	int		line_length;
	int		line_count;

	line_length = 0;
	line_count = 0;
	fd = open(MAP_FILE, O_RDONLY);
	if (fd < 0)
		exit_error(MSG_ERROR_MAP_FILE_ACCESS);
	while (check_map_line(fd, &line_length) > 0)
	{
		line_count++;
	}
	app->map_size.y = line_count;
	app->map_size.x = (line_length + 1) / 5;
	if (POSITION_START_X >= (double) app->map_size.x - 1.f
		|| POSITION_START_Y >= (double) app->map_size.y - 1.f)
		exit_error(MSG_ERROR_MAP_SIZE);
	return (1);
}

/**
 * Parses the map file and saves it into 3d char array.
*/
int	parse_map(t_app *app)
{
	int		fd;
	char	*buffer;
	int		map_size;

	map_size = app->map_size.y * app->map_size.x * 5;
	fd = open(MAP_FILE, O_RDONLY);
	if (fd < 0)
		exit_error(MSG_ERROR_MAP_FILE_ACCESS);
	buffer = (char *)malloc(sizeof(char) * map_size + 1);
	if (!buffer)
		exit_error(MSG_ERROR_ALLOC);
	ft_bzero(buffer, map_size + 1);
	if (read(fd, buffer, map_size) < 0 || close(fd) < 0)
		exit_error(MSG_ERROR_MAP_FILE_ACCESS);
	if (!map_is_valid(buffer, app))
		exit_error(MSG_ERROR_MAP_INVALID);
	save_map(app, buffer);
	free(buffer);
	return (1);
}
