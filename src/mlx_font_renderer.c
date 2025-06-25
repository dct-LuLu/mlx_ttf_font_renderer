/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_font_renderer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 22:13:23 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/25 16:58:50 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

static t_mlx	*init_mlx(void)
{
	t_mlx	*mlx;

	mlx = ft_calloc(1, sizeof(t_mlx));
	if (!mlx)
		return (NULL);
	mlx->mlx = mlx_init();
	if (!mlx->mlx)
		return (kill_mlx(mlx), NULL);
	mlx->win = mlx_new_window(mlx->mlx, WIDTH, HEIGHT, "font_renderer");
	if (!mlx->win)
		return (kill_mlx(mlx), NULL);
	ft_bzero(&mlx->img, sizeof(t_img));
	mlx->img = init_img(mlx->mlx, WIDTH, HEIGHT);
	if (!mlx->img.img)
		return (kill_mlx(mlx), NULL);
	mlx->ret = 0;
	mlx->origin = new_vec2(0, 0);
	mlx->size = new_vec2(WIDTH, HEIGHT);
	return (mlx);
}

void	*renderer_mainloop(t_env *env)
{
	env->zoom = 10;
	env->x = 0;
	env->y = 0;
	env->mlx = init_mlx();
	if (!env->mlx)
		return (NULL);
	mlx_hook(env->mlx->win, DestroyNotify, StructureNotifyMask, &kill_mlx,
		env->mlx);
	mlx_hook(env->mlx->win, KeyPress, KeyPressMask, &on_key_press, env);
	mlx_hook(env->mlx->win, ButtonPress, ButtonPressMask, &on_button_press, env);
	//mlx_hook(env->mlx->win, ButtonRelease, ButtonReleaseMask, &on_button_release, env);
	mlx_hook(env->mlx->win, MotionNotify, Button2MotionMask, &on_mwheel_drag, env);
	//mlx_mouse_hook(env->mlx->win, &mouse_handler, env);
	mlx_loop_hook(env->mlx->mlx, &draw_routine, env);
	mlx_loop(env->mlx->mlx);
	kill_mlx(env->mlx);
	return (NULL);
}
