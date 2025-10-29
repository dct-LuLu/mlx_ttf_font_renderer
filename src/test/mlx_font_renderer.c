/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_font_renderer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 22:13:23 by jaubry--          #+#    #+#             */
/*   Updated: 2025/10/29 00:34:38 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

int		on_key_press(int keysym, t_mlx *mlx);
int		draw_routine(t_rast_env *env);
void	add_text(t_rast_env *env, t_text *text);
void	init_fps(t_rast_env *env, t_ttf_font *font);

static void	add_test(t_rast_env *env, t_ttf_font *font)
{
	const int	size = 3;
	t_text	*text;

	text = ft_calloc(1, sizeof(t_text));
	ft_memcpy(text->content, "hihi test\nfeur :3\n^&(#$@)\n{}j,./:;", 36);
	text->size = size;
	text->pos = vec2i(WIDTH / 8, HEIGHT / 8 + size * 6);
	text->fg = (t_rgba_int){.rgba=WHITE};
	text->outline = (t_rgba_int){.rgba=0xA8FF0000};
	text->outlined = false;
	text->subpixel = false;
	text->font = font;
	text->img = &(env->mlx->img);
	add_text(env, text);
}

void	start_mainloop(t_rast_env *env, t_ttf_font *font)
{
	env->mlx = init_mlx(WIDTH, HEIGHT, "test");
	if (!env->mlx)
		return ;
	init_fps(env, font);
	add_test(env, font);
	ft_mlx_center_window(env->mlx);
	start_mlx_loop(env->mlx, draw_routine, env);
	//free text
}
