/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 17:49:38 by jaubry--          #+#    #+#             */
/*   Updated: 2025/07/16 19:47:48 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

/*
	Apply transformation matrix to a coordinate
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

/*
	Get transformed point from glyph coordinates
*/
// inline
t_vec2	get_transformed_point(t_glyf_table *glyf, int point_idx,
		t_glyf_component *transform)
{
	return (apply_transform(get_glyf_coords(glyf, point_idx), transform));
}
