/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scale_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 19:48:21 by jaubry--          #+#    #+#             */
/*   Updated: 2025/08/06 10:28:44 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"
#include "rasterizer.h"

/*
	Function that computes the position of a coordinate scaled based on the
	the text scale and the world zoom
*/
static float	apply_scale(float cord, int zoom, float scale)
{
	cord *= scale;
	if (zoom <= 0)
		return (cord * (-zoom + 1));
	return (cord / zoom);
}

/*
	Convert x coordinates to screen cordinates by
	scaling, zooming, translating.
*/
float	scale_x(t_text *text, int ctr_x, int glyph_x)
{
	float	scale;

	if (text->font->head->units_per_em == 0)
		return (0);
	scale = 500.0f / (float)text->font->head->units_per_em;
	return (apply_scale(glyph_x, text->size, scale) + ctr_x);
}

/*
	Convert y coordinates to screen cordinates by
	scaling, zooming, translating.
*/
float	scale_y(t_text *text, int ctr_y, int glyph_y)
{
	float	scale;

	if (text->font->head->units_per_em == 0)
		return (0);
	scale = 500.0f / (float)text->font->head->units_per_em;
	return (apply_scale(-glyph_y, text->size, scale) + ctr_y);
}

/*
	Convert coordinates to screen cordinates by
	scaling, zooming, translating.
*/
t_vec2i	to_screen_pt(t_text *text, t_vec2i glyph_pos, t_vec2i base_pos)
{
	if (text->font->head->units_per_em == 0)
		return (vec2i(0, 0));
	return (vec2i(scale_x(text, base_pos.x, glyph_pos.x),
			scale_y(text, base_pos.y, glyph_pos.y)));
}

//inline
t_vec2i	new_screen_pt(t_contour *contour, int x, int y)
{
	return (to_screen_pt(contour->text, vec2i(x, y), contour->pos));
}
