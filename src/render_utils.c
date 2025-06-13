/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:48:09 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/12 23:03:49 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"



/**
 * @brief Convert font coordinates to screen coordinates
 */
t_vec2	transform_coordinate(t_env *env, t_vec2 glyph_pos, t_vec2 base_pos)
{
	float	scale;
	float	x;
	float	y;

	if (env->font->head->units_per_em == 0)
		return (new_vec2(0, 0));
	scale = 500.0f / (float)env->font->head->units_per_em;
	x = glyph_pos.x + base_pos.x + env->x;
	y = -glyph_pos.y + base_pos.y + env->y;
	x *= scale;
	y *= scale;
	if (env->zoom == 0)
		return (new_vec2((int)x, (int)y));
	return (new_vec2((int)(x / env->zoom), (int)(y / env->zoom)));
}

/**
 * @brief Apply transformation matrix to a coordinate
 */
t_vec2	apply_transform(t_vec2 point, t_glyf_component *comp)
{
	t_vec2	result;

	if (!comp || !(comp->flags & (HAS_SCALE | HAS_XY_SCALE | HAS_2X2_MATRIX)))
		return (point);
	result.x = point.x * comp->transform[0] + point.y * comp->transform[2];
	result.y = point.x * comp->transform[1] + point.y * comp->transform[3];
	return (result);
}

t_vec2	new_screen_pt(t_contour *contour, int x, int y)
{
	t_vec2	pt;
	t_vec2	transformed;
	t_vec2	screen;

	pt = new_vec2(x, y);
	transformed = apply_transform(pt, contour->transform);
	screen = transform_coordinate(contour->env, transformed, contour->pos);
	return (screen);
}
