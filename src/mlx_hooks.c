/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_hooks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 22:32:25 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/15 21:49:25 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

/*
	Function that safely kills the mlx instance.
*/
int	kill_mlx(t_mlx *mlx)
{
	if (mlx)
	{
		if (mlx->mlx)
		{
			if (mlx->img.img)
				mlx_destroy_image(mlx->mlx, mlx->img.img);
			if (mlx->win)
				mlx_destroy_window(mlx->mlx, mlx->win);
			mlx_destroy_display(mlx->mlx);
			free(mlx->mlx);
		}
		free(mlx);
	}
	return (0);
}

/*
	Function to handles ESC key to exit safely.
*/
int	on_keypress(int keysym, t_env *env)
{
	if (keysym == 65307)
		return (mlx_loop_end(env->mlx->mlx));
	else if (keysym == XK_Left)
		env->x += 200;
	else if (keysym == XK_Up)
		env->y += 200;
	else if (keysym == XK_Right)
		env->x -= 200;
	else if (keysym == XK_Down)
		env->y -= 200;
	else if (keysym == XK_v)
		env->view_mode = (1 + env->view_mode) % 3;
	return (0);
}

int	mouse_handler(int mousecode, int x, int y, t_env *env)
{
	(void)x;
	(void)y;
	if (mousecode == 5)
		env->zoom *= 1.1;
	else if (mousecode == 4)
		env->zoom *= 0.9;
	return (0);
}
