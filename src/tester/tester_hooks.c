/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tester_hooks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 22:32:25 by jaubry--          #+#    #+#             */
/*   Updated: 2025/10/29 02:04:42 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

int	on_mwheel_drag(int x, int y, t_env *env)
{
	if (env->zoom <= 0)
	{
		env->x += (x - env->last_x) / ((-env->zoom) + 1);
		env->y += (y - env->last_y) / ((-env->zoom) + 1);
	}
	else
	{
		env->x += env->zoom * (x - env->last_x);
		env->y += env->zoom * (y - env->last_y);
	}
	if (DEBUG)
	{
		if ((x - env->last_x) > 0)
			printf("+");
		printf("%d\t", (x - env->last_x));
		if ((y - env->last_y) > 0)
			printf("+");
		printf("%d\n", (y - env->last_y));
	}
	env->last_x = x;
	env->last_y = y;
	return (0);
}

int	on_button_press(int mousecode, int x, int y, t_env *env)
{
	if (mousecode == 2)
	{
		env->last_x = x;
		env->last_y = y;
	}
	return (0);
}
