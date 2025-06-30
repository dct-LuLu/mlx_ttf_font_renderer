/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:48:09 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/30 15:22:16 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

/**
 * @brief Convert font coordinates to screen coordinates
 */

float	transform_x(t_contour *contour, int _x)
{
	float	scale;
	float	x;
	t_env	*env;
	
	env = contour->env;
	if (env->font->head->units_per_em == 0)
		return (0);
	scale = 500.0f / (float)env->font->head->units_per_em;
	x = _x + contour->pos.x + env->x;
	x *= scale;
	if (env->zoom <= 0)
		return (x * -env->zoom + 1);
	return (x / env->zoom);
}

int	transform_y(t_contour *contour, int _y)
{
	float	scale;
	float	y;
	t_env	*env;
	
	env = contour->env;
	if (env->font->head->units_per_em == 0 || env->zoom == 0)
		return (0);
	scale = 500.0f / (float)env->font->head->units_per_em;
	y = -_y + contour->pos.y + env->y;
	y *= scale;
	if (env->zoom <= 0)
		return (y * -env->zoom + 1);
	return (y / env->zoom);
}


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
	if (env->zoom <= 0)
		return (new_vec2((int)(x * -env->zoom + 1), (int)(y * -env->zoom + 1)));
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
	return (transform_coordinate(contour->env, new_vec2(x, y), contour->pos));
}


t_vec2	new_screen_pt2(t_contour *contour, t_vec2 v)
{
	return (transform_coordinate(contour->env, v, contour->pos));
}