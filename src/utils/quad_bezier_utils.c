/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quad_bezier_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 10:27:40 by jaubry--          #+#    #+#             */
/*   Updated: 2025/08/06 09:16:25 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "math_utils.h"

/*
	Function that returns the coordinates of a point based on the quadratic
	bezier curves coordinates of start, end and control point and uses
	t to travel from start to end.
*/
t_vec2i	quad_bezier_pt(t_vec2i start, t_vec2i ctrl, t_vec2i end, float t)
{
	float	m;
	float	a;
	float	b;
	float	c;

	m = (1 - t);
	a = m * m;
	b = 2 * m * t;
	c = t * t;
	return (vec2i(((a * start.x) + (b * ctrl.x) + (c * end.x)),
			((a * start.y) + (b * ctrl.y) + (c * end.y))));
}

/*
	Function to get the ideal resolution for a bezier curve for the
	minimal number of subdivision but with the best look for each possible
	curves.
*/
float	quad_bezier_res(t_vec2i start, t_vec2i ctrl, t_vec2i end)
{
	const float	a = fast_distance(start, ctrl);
	const float	b = fast_distance(ctrl, end);
	const float	c = fast_distance(start, end);

	if (((a + b) == 0) || (c == 0))
		return (-1);
	return (clamp(((a + b) / c) / CURVE_RESOLUTION, 0.01f, 0.99f));
}

float	quad_curve_length(t_vec2i start, t_vec2i ctrl, t_vec2i end)
{
	const float	chord_length = fast_distance(start, end);
	const float	ctrl_length = fast_distance(start, ctrl)
		+ fast_distance(ctrl, end);

	return ((chord_length + ctrl_length) * 0.5f);
}
