/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:48:09 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/27 17:00:34 by jaubry--         ###   ########.fr       */
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

	scale = 500.0f / (float)env->font->head->units_per_em;
	x = glyph_pos.x + base_pos.x + env->x;
	y = -glyph_pos.y + base_pos.y + env->y;
	x *= scale;
	y *= scale;
	return (new_vec2(x / env->zoom, y / env->zoom));
}

/**
 * @brief Apply transformation matrix to a coordinate
 */
t_vec2 apply_transform(t_vec2 point, t_glyf_component *comp)
{
    t_vec2 result;
    
    if (!comp || !(comp->flags & (HAS_SCALE | HAS_XY_SCALE | HAS_2X2_MATRIX)))
        return (point);
    result.x = point.x * comp->transform[0] + point.y * comp->transform[2];
    result.y = point.x * comp->transform[1] + point.y * comp->transform[3];
    return (result);
}


/**
 * @brief Calculate component position with offsets and scaling
 */
t_vec2 get_component_position(t_vec2 base_pos, t_glyf_component *comp)
{
    t_vec2 comp_pos;
    t_vec2 offset;
    
    comp_pos = base_pos;
    if (!comp || !(comp->flags & ARGS_ARE_XY_VALUES))
        return (comp_pos);
    offset = new_vec2(comp->arg1, comp->arg2);
    if (comp->flags & SCALED_COMPONENT_OFFSET)
        offset = apply_transform(offset, comp);
    comp_pos.x += offset.x;
    comp_pos.y += offset.y;
    return (comp_pos);
}
