/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keybinds_linux.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saaltone <saaltone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 15:57:29 by saaltone          #+#    #+#             */
/*   Updated: 2022/07/11 15:59:57 by saaltone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEYBINDS_LINUX_H
# define KEYBINDS_LINUX_H

enum e_keybinds {
	KEY_ESC = 65307,
	KEY_W = 119,
	KEY_A = 97,
	KEY_S = 115,
	KEY_D = 100,
	KEY_F = 102,
	KEY_H = 104,
	KEY_U = 117,
	KEY_I = 105,
	KEY_ARROW_UP = 65362,
	KEY_ARROW_DOWN = 65364,
	KEY_ARROW_LEFT = 65361,
	KEY_ARROW_RIGHT = 65363,
	KEY_PGUP = 65365,
	KEY_PGDOWN = 65366
};

enum e_mousebinds {
	MOUSE_CLICK_LEFT = 1,
	MOUSE_SCROLL_UP = 4,
	MOUSE_SCROLL_DOWN = 5
};

#endif