/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_text.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 22:00:00 by jaubry--          #+#    #+#             */
/*   Updated: 2025/08/06 10:28:06 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"
#include "rasterizer.h"

void	draw_glyph(t_contour *contour);

static t_vec2i	advance_pen_position(t_vec2i pen_pos, t_contour ctr)
{
	uint16_t	advance_width;
	t_vec2i		new_pos;

	new_pos = pen_pos;
	if (!ctr.text->font->hmtx || ctr.glyf_idx < 0)
		return (new_pos);
	if (ctr.glyf_idx < ctr.text->font->hmtx->num_lhmtx)
		advance_width = ctr.text->font->hmtx->lhmtx[ctr.glyf_idx].advance_width;
	else
		advance_width = ctr.text->font->hmtx->lhmtx
		[ctr.text->font->hmtx->num_lhmtx - 1]
			.advance_width;
	new_pos.x += scale_x(ctr.text, 0, advance_width);
	return (new_pos);
}

static t_vec2i	draw_character(t_contour ctr, t_vec2i pen_pos)
{
	t_vec2i	new_pen_pos;
	int		xpt[2];
	int		y;

	ctr.pos = pen_pos;
	new_pen_pos = advance_pen_position(pen_pos, ctr);
	if (ctr.glyf)
		draw_glyph(&ctr);
	if (DEBUG)
	{
		xpt[0] = scale_x(ctr.text, pen_pos.x, ctr.pos.x);
		xpt[1] = scale_x(ctr.text, new_pen_pos.x, ctr.pos.x);
		y = scale_y(ctr.text, -pen_pos.y, ctr.pos.y);
		ft_mlx_horizontal_line(ctr.text->img, xpt, y, RED);
	}
	return (new_pen_pos);
}

static t_contour	get_contour(t_text *text, char c)
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
	ctr.pos = text->pos;
	return (ctr);
}

void	draw_text(t_text *text)
{
	t_vec2i		pen_pos;
	t_contour	ctr;
	size_t		i;

	pen_pos = text->pos;
	i = 0;
	while (text->content[i])
	{
		if (text->content[i] == '\n')
		{
			pen_pos.y += scale_y(text, 0, abs(text->font->head->y_min)
					+ text->font->head->y_max);
			pen_pos.x = text->pos.x;
		}
		else
		{
			ft_bzero(&ctr, sizeof(ctr));
			ctr = get_contour(text, text->content[i]);
			pen_pos = draw_character(ctr, pen_pos);
		}
		i++;
	}
}
