/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hooks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 01:15:00 by jaubry--          #+#    #+#             */
/*   Updated: 2025/10/29 05:39:06 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"
#include "tester.h"

static bool	is_arrow_keys(int keysym)
{
	return ((keysym == XK_Left) || (keysym == XK_Up)
			|| (keysym == XK_Right) || (keysym == XK_Down));
}

static bool	is_v_key(int keysym)
{
	return (keysym == XK_v);
}

static bool	is_p_key(int keysym)
{
	return (keysym == XK_p);
}

static bool	is_s_key(int keysym)
{
	return (keysym == XK_s);
}

static bool	is_l_key(int keysym)
{
	return (keysym == XK_l);
}

static void	move_view(t_env *env, t_mlx *mlx_data)
{
	if (mlx_data->key_input.keycode == XK_Left)
		env->x += 200;
	if (mlx_data->key_input.keycode == XK_Up)
		env->y += 200;
	if (mlx_data->key_input.keycode == XK_Right)
		env->x -= 200;
	if (mlx_data->key_input.keycode == XK_Down)
		env->y -= 200;
	printf("pos: {.x=%d, .y=%d}\n",env->x,env->y);
}

static void	change_view_mode(t_env *env, t_mlx *mlx_data)
{
	(void)mlx_data;
	env->view_mode = (1 + env->view_mode) % 3;
}

static char	get_char_input(int keycode, bool maj)
{
	char	c;

	c = '\0';
	if (is_char_key(keycode))
	{
		if (is_tkl_char_key(keycode))
		{
			if (maj)
				c = MAX_CHAR[keycode - ' '];
			else
				c = MIN_CHAR[keycode - ' '];
		}
		else if (is_tab_key(keycode))
			c = '\t';
		else if (is_enter_key(keycode))
			c = '\n';
		else if (is_numpad_char_key(keycode))
			c = NUMPAD_CHAR[keycode - 0xff95];
	}
	return (c);
}

// add move and remove keys
static void	action_char_input(t_env *env, t_mlx *mlx)
{
	char	c;

	if (env->view_mode != EDIT_VIEW)
		return ;
	c = get_char_input(mlx->key_input.keycode, mlx->key_input.caps ^ mlx->key_input.shift);
	if (env->cur_pos < (MAX_CHARS - 1))
	{
		env->text->content[env->cur_pos] = c;
		if (DEBUG)
			printf("%d: '%c'\n", mlx->key_input.keycode, c);
		env->cur_pos++;
	}
}

static bool	is_backspace_key(int keycode)
{
	return (keycode == XK_BackSpace);
}

static void	action_char_backspace(t_env *env, t_mlx *mlx)
{
	(void)mlx;
	if (env->view_mode != EDIT_VIEW)
		return ;
	if (env->cur_pos > 0)
	{
		env->text->content[env->cur_pos - 1] = 0;
		env->cur_pos--;
	}
}

int	setup_tester_key_events(t_env *env)
{
	if (add_func_key_hook(env->mlx, is_char_key, (void (*)(void *, t_mlx *))action_char_input, env) != 0)
		return (error(pack_err(MLXW_ID, MLXW_E_EVENTH), FL, LN, FC));
	if (add_func_key_hook(env->mlx, is_backspace_key, (void (*)(void *, t_mlx *))action_char_backspace, env) != 0)
		return (error(pack_err(MLXW_ID, MLXW_E_EVENTH), FL, LN, FC));
	if (add_func_key_hook(env->mlx, is_arrow_keys, (void (*)(void *, t_mlx *))move_view, env) != 0)
		return (error(pack_err(MLXW_ID, MLXW_E_EVENTH), FL, LN, FC));
	if (add_func_key_hook(env->mlx, is_v_key, (void (*)(void *, t_mlx *))change_view_mode, env) != 0)
		return (error(pack_err(MLXW_ID, MLXW_E_EVENTH), FL, LN, FC));
	if (add_status_key_hook(env->mlx, is_p_key, true, &env->subpixel) != 0)
		return (error(pack_err(MLXW_ID, MLXW_E_EVENTH), FL, LN, FC));
	if (add_func_key_hook(env->mlx, is_s_key, (void (*)(void *, t_mlx *))save_env_state, env) != 0)
		return (error(pack_err(MLXW_ID, MLXW_E_EVENTH), FL, LN, FC));
	if (add_func_key_hook(env->mlx, is_l_key, (void (*)(void *, t_mlx *))load_env_state, env) != 0)
		return (error(pack_err(MLXW_ID, MLXW_E_EVENTH), FL, LN, FC));
	return (0);
}
