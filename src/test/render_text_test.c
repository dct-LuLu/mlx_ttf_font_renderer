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

t_contour	get_contour(t_text *text, char c);
void		draw_glyph(t_contour *contour);

void	render_text_test(t_text *text)
{
	t_contour	ctr;
	t_vec2i		pen_pos;
	int			char_width;
	size_t		i;

	i = 0;
	pen_pos = text->_text_pos;
	while (text->content[i])
	{
		char_width = measure_char_width(text->content[i], text);
		if (text->content[i] == '\n')
			pen_pos = vec2i(text->_lt_limit.x, pen_pos.y - scale_y(text, 0,
				abs(text->font->head->y_min) + text->font->head->y_max));
		else
		{
			ft_bzero(&ctr, sizeof(ctr));
			ctr = get_contour(text, text->content[i]);
			ctr.pos = pen_pos;
			if (ctr.glyf)
				draw_glyph(&ctr);
			pen_pos.x += char_width;
		}
		i++;
	}
}