/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_font_renderer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 22:13:23 by jaubry--          #+#    #+#             */
/*   Updated: 2025/11/04 18:31:03 by jaubry--         ###   ########.fr       */
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
	ft_memcpy(env->test->content, "\"explique\" hahaha pff ouais c'est un peu chiant les gars en gros Luden c'est un mythique qui donne de la pene magique et donc en en gros ca donne 6 de pene magique flat donc a 2 items complets.. donc il a 10 de pene flat donc il monte a 16, il a les bottes ca fait 18. Donc 16+18 ca fait 34 si jdis pas de conneries donc 34 + il avait shadow flame donc il a 44 et il a 44 et apres du coup le void staff faut faire 44 divise par 0.6 en gros il fait des degats purs a un mec jusqu'a 73 d'rm j'avais dit 70 dans le cast a peu pres et en gros bah les mecs ils ont pas 70 d'rm parce que globalement y'a eu un patch, en gros y'a le patch qui fait 0.8 d'rm sur les carrys et en gros de base sur lol y'avait pas ca et en gros la botlane va jamais prendre de la rm en lane en tout cas pas beaucoup donc c'est pas ouf en vrai j'pense que son item est nul donc en vrai j'pense soit il enleve shadow flame soit le void staff mais j'pense qu'il vaut mieux enlever shadow flame", 963);
	env->test->font_size = size;
	env->test->pos = vec2i(0, 0);
	env->test->lt = env->test->pos;
	env->test->rb = env->test->lt;
	env->test->fg = (t_rgba_int){.rgba=WHITE};
	env->test->outline = (t_rgba_int){.rgba=0xA8FF0000};
	env->test->outlined = false;
	env->test->subpixel = true;
	env->test->font = env->font;
	env->test->img = &(env->mlx_data->img);
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
