/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_font_renderer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 22:13:23 by jaubry--          #+#    #+#             */
/*   Updated: 2025/11/05 13:34:07 by jaubry--         ###   ########.fr       */
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
	const int	size = 2;

	env->test = ft_calloc(1, sizeof(t_text));
	ft_memcpy(env->test->content, "ceci\nest\nun\ntest\nvoila\t\tc'est tout bonjour\n nan mais pourquoi \n adddsdsdfsee\n\n\n\nfdsfs", 500);
	env->test->font_size = size;
	env->test->pos = vec2i(WIDTH/4, HEIGHT/4);
	env->test->lt = env->test->pos;
	env->test->rb = env->test->lt;
	env->test->fg = (t_rgba_int){.rgba=WHITE};
	env->test->outline = (t_rgba_int){.rgba=0xA8FF0000};
	env->test->outlined = false;
	env->test->subpixel = true;
	env->test->font = env->font;
	env->test->img = &(env->mlx_data->img);
	env->test->align = RIGHT_ALIGN;
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
