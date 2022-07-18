/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_key.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 14:15:51 by saaltone          #+#    #+#             */
/*   Updated: 2022/07/15 16:25:00 by saaltone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
 * Checks for collisions for movement.
*/
static int	is_collision(t_app *app, t_vector2 pos)
{
	if (pos.x >= app->map_size.x - 1 || pos.y >= app->map_size.y - 1)
		return (1);
	if (pos.x < 0 || pos.y < 0)
		return (1);
	if (app->map[(int)pos.y][(int)pos.x][0] > '0')
		return (1);
	return (0);
}

static void	handle_player_direction(int keycode, t_app *app)
{
	if (keycode == KEY_ARROW_RIGHT)
		player_rotate(app, ROTATION);
	if (keycode == KEY_ARROW_LEFT)
		player_rotate(app, -ROTATION);
}

static void	handle_player_position(int keycode, t_app *app)
{
	t_vector2	perpendicular;
	t_vector2	new_pos;

	if (keycode == KEY_ARROW_UP || keycode == KEY_W)
		new_pos = (t_vector2){app->player.position.x + app->player.direction.x * MOVEMENT,
				app->player.position.y + app->player.direction.y * MOVEMENT};
	if (keycode == KEY_ARROW_DOWN || keycode == KEY_S)
		new_pos = (t_vector2){app->player.position.x - app->player.direction.x * MOVEMENT, 
			app->player.position.y - app->player.direction.y * MOVEMENT};
	if (keycode == KEY_A || keycode == KEY_D)
	{
		perpendicular = ft_vector_perpendicular(app->player.direction);
		if (keycode == KEY_A)
			new_pos = (t_vector2){app->player.position.x - perpendicular.x * MOVEMENT, 
				app->player.position.y - perpendicular.y * MOVEMENT};
		if (keycode == KEY_D)
			new_pos = (t_vector2){app->player.position.x + perpendicular.x * MOVEMENT, 
				app->player.position.y + perpendicular.y * MOVEMENT};
	}
	if ((keycode == KEY_ARROW_UP || keycode == KEY_ARROW_DOWN || keycode == KEY_W || keycode == KEY_S || keycode == KEY_A || keycode == KEY_D) && !is_collision(app, new_pos))
		app->player.position = new_pos;
}

/*
 * Handles events for key presses (keyup).
*/
int	events_keyup(int keycode, t_app *app)
{
	ft_printf("Keyup: %i\n", keycode);
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
	ft_printf("Keydown: %i\n", keycode);
	handle_player_direction(keycode, app);
	handle_player_position(keycode, app);
	if (keycode == KEY_U)
		app->conf->fov--;
	if (keycode == KEY_I)
		app->conf->fov++;
	if (keycode == KEY_U || keycode == KEY_I)
		init_camera_plane(app);
	app_render(app);
	return (0);
}
