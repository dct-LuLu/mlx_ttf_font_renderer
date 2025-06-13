/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_curve_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 21:37:58 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/13 22:34:07 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

/*
	Function that subdivide the curve into smaller segments
*/
static void	subdivide_quadratic_curve(t_vec2 p0, t_vec2 p1, t_vec2 p2,
		t_vec2 result[5])
{
	t_vec2	q0;
	t_vec2	q1;
	t_vec2	r;

	q0.x = (p0.x + p1.x + 1) * 0.5;
	q0.y = (p0.y + p1.y + 1) * 0.5;
	q1.x = (p1.x + p2.x + 1) * 0.5;
	q1.y = (p1.y + p2.y + 1) * 0.5;
	r.x = (q0.x + q1.x + 1) * 0.5;
	r.y = (q0.y + q1.y + 1) * 0.5;
	result[0] = p0;
	result[1] = q0;
	result[2] = r;
	result[3] = q1;
	result[4] = p2;
}

/*
	Check if curve is degenerate (all points nearly collinear)
	If cross product is very small relative to chord length, it's degenerate
*/
static int	is_degenerate_curve(t_vec2 p0, t_vec2 p1, t_vec2 p2)
{
	int	chord_length_sq;
	int	cross_product;
	int	dxy1[2];
	int	dxy2[2];

	dxy1[0] = p1.x - p0.x;
	dxy1[1] = p1.y - p0.y;
	dxy2[0] = p2.x - p0.x;
	dxy2[1] = p2.y - p0.y;
	cross_product = abs(dxy1[0] * dxy2[1] - dxy1[1] * dxy2[0]);
	chord_length_sq = dxy2[0] * dxy2[0] + dxy2[1] * dxy2[1];
	return (cross_product * cross_product < chord_length_sq / 4);
}

/*
	Function that computes a chord length, and checks if its flat or too small.
*/
static bool	is_curve_flat(t_vec2 p0, t_vec2 p1, t_vec2 p2, int depth)
{
	int	flatness_check;
	int	chord_length_sq;
	int	dxy[2];
	int	threshold;

	dxy[0] = p2.x - p0.x;
	dxy[1] = p2.y - p0.y;
	chord_length_sq = dxy[0] * dxy[0] + dxy[1] * dxy[1];
	if (chord_length_sq < 4)
		return (true);
	flatness_check = abs((p1.x - p0.x) * (p2.y - p0.y) - (p1.y - p0.y) * (p2.x
				- p0.x));
	if (chord_length_sq < 100)
		threshold = 2;
	else
		threshold = 8;
	threshold += depth;
	if (flatness_check < threshold)
		return (true);
	return (false);
}

/*
** Recursive curve subdivision with maximum depth limit and degenerate detection
*/
void	add_curve_fill(t_fill_data *fill, t_contour *contour, t_curve_params params, int contour_direction, int depth)
{
	t_vec2	subdivided[5];

	if ((depth > 12) || (is_degenerate_curve(params.start_pt, params.control_pt, params.end_pt)))
		return (add_edge(fill, params.start_pt, params.end_pt, contour_direction));
	if (is_curve_flat(params.start_pt, params.control_pt, params.end_pt, depth))
		return (add_edge(fill, params.start_pt, params.end_pt, contour_direction));
	subdivide_quadratic_curve(params.start_pt, params.control_pt, params.end_pt, subdivided);
	params.start_pt = subdivided[0];
	params.control_pt = subdivided[1];
	params.end_pt = subdivided[2];
	add_curve_fill(fill, contour, params, contour_direction, depth + 1);
	params.start_pt = subdivided[2];
	params.control_pt = subdivided[3];
	params.end_pt = subdivided[4];
	add_curve_fill(fill, contour, params, contour_direction, depth + 1);
}
