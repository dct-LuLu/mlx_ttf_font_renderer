/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_hooks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 03:59:49 by jaubry--          #+#    #+#             */
/*   Updated: 2025/10/29 05:32:06 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"
#include "tester.h"

static void	zoom_in_action(t_vec2i pos, t_maction action, t_env *env, t_mlx *mlx)
{
	(void)pos;
	(void)mlx;
	int	incr;

	if (action == MRELEASE)
		return ;
	incr = 1;
	if (env->zoom > 100)
		incr = 5;
	if (env->zoom < 800)
		env->zoom += incr;
}

static void	zoom_out_action(t_vec2i pos, t_maction action, t_env *env, t_mlx *mlx)
{
	(void)pos;
	(void)mlx;

	if (action == MRELEASE)
		return ;
	if (env->zoom > 1)
		env->zoom -= 1;
}

int	setup_tester_mouse_events(t_env *env)
{
	if (add_func_button_hook(env->mlx, MWHEELUP, (void (*)(t_vec2i, t_maction, void *, t_mlx *))zoom_in_action, env) != 0)
		return (error(pack_err(MLXW_ID, MLXW_E_EVENTH), FL, LN, FC));
	if (add_func_button_hook(env->mlx, MWHEELDOWN, (void (*)(t_vec2i, t_maction, void *, t_mlx *))zoom_out_action, env) != 0)
		return (error(pack_err(MLXW_ID, MLXW_E_EVENTH), FL, LN, FC));
	return (0);
}
