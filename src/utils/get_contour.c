/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_text.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 22:00:00 by jaubry--          #+#    #+#             */
/*   Updated: 2025/11/06 12:24:21 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"
#include "rasterizer.h"

t_contour	get_contour(t_text *text, char c)
{
	t_contour	ctr;

	ft_bzero(&ctr, sizeof(t_contour));
	if (c == '\0')
	{
		ctr.glyf_idx = -1;
		return (ctr);
	}
	ctr.glyf_idx = get_glyph_index(text->font, c);
	if (ctr.glyf_idx >= text->font->maxp->num_glyphs)
		ctr.glyf_idx = 0;
	ctr.text = text;
	ctr.glyf = text->font->glyfs[ctr.glyf_idx];
	ctr.pos = text->_text_pos;
	return (ctr);
}