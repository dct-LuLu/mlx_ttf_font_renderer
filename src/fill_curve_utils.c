/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_curve_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 21:37:58 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/29 18:34:23 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

/*
	Function that subdivide the curve into smaller segments
*/
/*
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

*/
/*
	Check if curve is degenerate (all points nearly collinear)
	If cross product is very small relative to chord length, it's degenerate
*/

/*
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

*/

/*
	Function that computes a chord length, and checks if its flat or too small.
*/
/*
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
*/

t_vec2	quad_bezier_pt(t_vec2 start, t_vec2 ctrl, t_vec2 end, float t)
{
	float	m;
	float	a;
	float	b;
	float	c;

	m = (1 - t);
	a = m * m;
	b = 2 * m * t;
	c = t * t;
	return (new_vec2(((a * start.x) + (b * ctrl.x) + (c * end.x)),
		((a * start.y) + (b * ctrl.y) + (c * end.y))));
}

/*
	Euclidean distance, achieving approximately 3.5% error rate.
*/
int	fast_distance(t_vec2 a, t_vec2 b)
{
	const int	dx = abs(b.x - a.x);
	const int	dy = abs(b.y - a.y);
	int			min_val;

	if (dx < dy)
		min_val = dx;
	else
		min_val = dy;
	return (dx + dy - (min_val >> 1) - (min_val >> 2) + (min_val >> 4));
}

#include <math.h>
//inline
float	clamp(float value, float min_val, float max_val)
{
    return (fmaxf(min_val, fminf(value, max_val)));
}

float	quad_bezier_res(t_curve_params params)
{
	const float	a = fast_distance(params.start_pt, params.ctrl_pt);
	const float	b = fast_distance(params.ctrl_pt, params.end_pt);
	const float	c = fast_distance(params.start_pt, params.end_pt);

	if (((a + b) == 0) || (c == 0))
		return (-1);
	return (clamp(((a + b) / c) / 4.0f, 0.01f, 0.99f));
}

/*
	Recursive curve subdivision with maximum depth limit and degenerate detection
*/
void	add_curve_fill(t_fill_data *fill, t_contour *contour,
		t_curve_params params)
{
	const float	res = quad_bezier_res(params);
	float	t;
	t_vec2	a;
	t_vec2	b;

	a = params.start_pt;
	t = 0.0f;
	while (t < 1)
	{
		b = quad_bezier_pt(params.start_pt, params.ctrl_pt, params.end_pt, t);
		if (DEBUG && (contour->env->zoom <= 0))
		{
			ft_mlx_circle_put(&contour->env->mlx->img, new_screen_pt(contour, a.x / 3, a.y),
				10, GREEN);
			ft_mlx_circle_put(&contour->env->mlx->img, new_screen_pt(contour, b.x / 3, b.y),
				10, GREEN);
			ft_mlx_line_put(&contour->env->mlx->img, new_screen_pt(contour, a.x / 3, a.y), new_screen_pt(contour, b.x / 3, b.y), GREEN);
		}
		add_edge(fill, a, b);
		a = b;
		t += res;
	}
	add_edge(fill, a, params.end_pt);
}
