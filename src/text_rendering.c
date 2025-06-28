/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text_rendering.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 22:00:00 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/28 12:33:40 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

t_vec2	transform_coordinate(t_env *env, t_vec2 glyph_pos, t_vec2 base_pos);
void	draw_glyph(t_contour *contour);

static t_vec2	advance_pen_position(t_vec2 pen_pos, t_contour ctr, t_env *env)
{
	uint16_t	advance_width;
	t_vec2		new_pos;

	new_pos = pen_pos;
	if (!env->font->hmtx || ctr.glyf_idx < 0)
		return (new_pos);
	if (ctr.glyf_idx < env->font->hmtx->num_lhmtx)
		advance_width = env->font->hmtx->lhmtx[ctr.glyf_idx].advance_width;
	else
		advance_width = env->font->hmtx->lhmtx[env->font->hmtx->num_lhmtx - 1]
			.advance_width;
	new_pos.x += advance_width;
	return (new_pos);
}

static t_vec2	draw_character(t_contour ctr, t_vec2 pen_pos)
{
	t_vec2	new_pen_pos;
	int		xpt[2];
	int		y;

	ctr.pos = pen_pos;
	if (ctr.glyf)
		draw_glyph(&ctr);
	new_pen_pos = advance_pen_position(pen_pos, ctr, ctr.env);
	if (DEBUG)
	{
		xpt[0] = transform_coordinate(ctr.env, pen_pos, new_vec2(0, 0)).x;
		xpt[1] = transform_coordinate(ctr.env, new_pen_pos, new_vec2(0, 0)).x;
		y = transform_coordinate(ctr.env, new_vec2(pen_pos.x, -pen_pos.y), new_vec2(0, 200)).y;
		ft_mlx_horizontal_line(&ctr.env->mlx->img, xpt, y, RED);
	}
	return (new_pen_pos);
}

static t_contour	get_countour(t_env *env, char c, int color)
{
	t_contour	ctr;

	ft_bzero(&ctr, sizeof(t_contour));
	if (c == '\0')
	{
		ctr.glyf_idx = -1;
		return (ctr);
	}
	ctr.glyf_idx = get_glyph_index(env->font, c);
	if (ctr.glyf_idx >= env->font->maxp->num_glyphs)
		ctr.glyf_idx = 0;
	ctr.env = env;
	ctr.color = color;
	ctr.glyf = env->font->glyfs[ctr.glyf_idx];
	return (ctr);
}

void	draw_string(t_env *env, const char *str, t_vec2 pos, int color)
{
	t_vec2		pen_pos;
	t_contour	ctr;
	int			i;

	if (!str || !env || !env->font)
		return ;
	pen_pos = pos;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\n')
		{
			pen_pos.y += abs(env->font->head->y_min) + env->font->head->y_max;
			pen_pos.x = pos.x;
		}
		else
		{
			ctr = get_countour(env, str[i], color);
			pen_pos = draw_character(ctr, pen_pos);
		}
		i++;
	}
}
