/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text_rendering.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 22:00:00 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/24 04:57:56 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

void	draw_glyph(t_contour *contour);

static t_vec2 advance_pen_position(t_vec2 pen_pos, t_contour cur, t_contour next, t_env *env)
{
	(void)next;
    uint16_t advance_width;
    t_vec2 new_pos = pen_pos;

    if (!cur.glyf || !env->font->hmtx || cur.glyf_idx < 0)
		return (new_pos);

    if (cur.glyf_idx < env->font->hmtx->num_lhmtx)
        advance_width = env->font->hmtx->lhmtx[cur.glyf_idx].advance_width;
    else
        advance_width = env->font->hmtx->lhmtx[env->font->hmtx->num_lhmtx - 1].advance_width;

    new_pos.x += advance_width;
    return (new_pos);
}


static t_vec2 draw_character(t_contour cur, t_contour next, t_vec2 pen_pos)
{
    cur.pos = pen_pos;
    
    if (cur.glyf)
        draw_glyph(&cur);
    
    return (advance_pen_position(pen_pos, cur, next, cur.env));
}


t_contour	get_countour(t_env *env, char c, t_vec2 pos, int color)
{
	t_contour	ctr;

	(void)pos;
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
	t_vec2	pen_pos;
	t_contour	cur;
	t_contour	next;
	int		i;

	if (!str || !env || !env->font)
		return ;
	pen_pos = pos;
	i = 0;
	while (str[i])
	{
		cur = get_countour(env, str[i], pos, color);
		next = get_countour(env, str[i + 1], pos, color);
		pen_pos = draw_character(cur, next, pen_pos);
		i++;
	}
}
