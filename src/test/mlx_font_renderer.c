/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_font_renderer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 22:13:23 by jaubry--          #+#    #+#             */
/*   Updated: 2025/11/06 11:02:13 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

typedef struct s_test_env
{
	t_ttf_font	*font;
	t_text		*fps;
	t_text		*test;
	t_mlx		*mlx_data;
}	t_test_env;

int		on_key_press(int keysym, t_mlx *mlx);
int		draw_routine(t_mlx *mlx_data, t_text *text, t_text *fps);
void	init_fps(t_test_env *env);

static void	add_test(t_test_env *env)
{
	env->test = ft_calloc(1, sizeof(t_text));
	ft_memcpy(env->test->content, "ceci\nest\nu\nn\nt\ne\nst\nvoila\nc'est to\nut bonjour\n nan mais pourquoi \n addd\nsds\ndf\nsee\nfdsfs", 500);
	env->test->font_size = 2;
	env->test->_text_pos = vec2i(WIDTH/4, HEIGHT/4 + env->test->font_size * 6);
	env->test->_lt_limit = env->test->_text_pos;
	env->test->_rb_limit = env->test->_lt_limit;
	env->test->fg = (t_rgba_int){.rgba=WHITE};
	env->test->outline = (t_rgba_int){.rgba=0xA8FF0000};
	env->test->outlined = false;
	env->test->subpixel = true;
	env->test->font = env->font;
	env->test->_img = &(env->mlx_data->img);
}

void	start_mainloop(t_ttf_font *font)
{
	t_test_env	env;
	
	env.font = font;
	env.mlx_data = init_mlx(WIDTH, HEIGHT, "test");
	if (!env.mlx_data)
		return ;
	init_fps(&env);
	add_test(&env);
	ft_mlx_center_window(env.mlx_data);
	start_mlx_loop(env.mlx_data, draw_routine, &env);
	//free text
}
