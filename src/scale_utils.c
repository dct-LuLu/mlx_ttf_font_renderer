/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scale_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 19:48:21 by jaubry--          #+#    #+#             */
/*   Updated: 2025/07/16 20:12:52 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

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
float	scale_x(t_env *env, int _x, int glyph_x)
{
	float	scale;
	float	x;

	if (env->font->head->units_per_em == 0)
		return (0);
	scale = 500.0f / (float)env->font->head->units_per_em;
	x = _x + glyph_x + env->x;
	return (apply_scale(x, env->zoom, scale));
}

/*
	Convert y coordinates to screen cordinates by
	scaling, zooming, translating.
*/
float	scale_y(t_env *env, int _y, int glyph_y)
{
	float	scale;
	float	y;

	if (env->font->head->units_per_em == 0)
		return (0);
	scale = 500.0f / (float)env->font->head->units_per_em;
	y = _y + -glyph_y + env->y;
	return (apply_scale(y, env->zoom, scale));
}

/*
	Convert coordinates to screen cordinates by
	scaling, zooming, translating.
*/
t_vec2	to_screen_pt(t_env *env, t_vec2 glyph_pos, t_vec2 base_pos)
{
	float	scale;

	if (env->font->head->units_per_em == 0)
		return (new_vec2(0, 0));
	scale = 500.0f / (float)env->font->head->units_per_em;
	return (new_vec2(scale_x(env, base_pos.x, glyph_pos.x),
			scale_y(env, base_pos.y, glyph_pos.y)));
}

//inline
t_vec2	new_screen_pt(t_contour *contour, int x, int y)
{
	return (to_screen_pt(contour->env, new_vec2(x, y), contour->pos));
}
