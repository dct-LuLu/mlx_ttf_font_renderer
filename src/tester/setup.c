/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 22:13:23 by jaubry--          #+#    #+#             */
/*   Updated: 2025/11/04 16:54:01 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"
#include "tester.h"

int		on_key_press(int keysym, t_mlx *mlx);
int		draw_routine(t_env *env);
void	add_text(t_env *env, t_text *text);
void	init_fps(t_env *env, t_ttf_font *font);

static void	add_test(t_env *env, t_ttf_font *font)
{
	env->text = ft_calloc(1, sizeof(t_text));
	*(env->text) = (t_text)
	{
		.pos = vec2i(0, 200),
		.font_size = 10,
		.fg = (t_rgba_int){.rgba=WHITE},
		.outline = (t_rgba_int){.rgba=WHITE},
		.outlined = false,
		.subpixel = false,
		.font = font,
		.img = &(env->mlx->img)
	};
}

int	setup_tester_key_events(t_env *env);
int	setup_tester_mouse_events(t_env *env);

void	start_mainloop(t_env *env)
{
	env->zoom = 1;
	env->x = 50;
	env->y = 50;
	load_env_state(env, env->mlx);
	env->mlx = init_mlx(WIDTH, HEIGHT, "tester");
	if (!env->mlx)
		return ;
	init_fps(env, env->font);
	add_test(env, env->font);
	ft_mlx_center_window(env->mlx);
	setup_tester_key_events(env);
	setup_tester_mouse_events(env);
	start_mlx_loop(env->mlx, draw_routine, env);
	//free text
}
