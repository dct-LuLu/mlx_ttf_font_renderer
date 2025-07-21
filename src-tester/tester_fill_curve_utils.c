/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_curve_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 21:37:58 by jaubry--          #+#    #+#             */
/*   Updated: 2025/07/16 19:26:19 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"
#include "math_utils.h"

/*
	Function that returns the coordinates of a point based on the quadratic
	bezier curves coordinates of start, end and control point and uses
	t to travel from start to end.
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
	Function to get the ideal resolution for a bezier curve for the
	minimal number of subdivision but with the best look for each possible
	curves.
*/
float	quad_bezier_res(t_curve_params params)
{
	const float	a = fast_distance(params.start_pt, params.ctrl_pt);
	const float	b = fast_distance(params.ctrl_pt, params.end_pt);
	const float	c = fast_distance(params.start_pt, params.end_pt);

	if (((a + b) == 0) || (c == 0))
		return (-1);
	return (clamp(((a + b) / c) / CURVE_RESOLUTION, 0.01f, 0.99f));
}

static void	debug_curve_subdivisions(t_contour *contour, t_vec2 _a, t_vec2 _b)
{
	t_vec2	a;
	t_vec2	b;

	a = to_screen_pt(contour->env, _a, contour->pos);
	b = to_screen_pt(contour->env, _b, contour->pos);
	if (DEBUG && (contour->env->zoom <= 0))
	{
		ft_mlx_circle_put(&contour->env->mlx->img, a, 7, GREEN);
		ft_mlx_circle_put(&contour->env->mlx->img, b, 7, GREEN);
		ft_mlx_line_put(&contour->env->mlx->img, a, b, GREEN);
	}
}

/*
	Recursive curve subdivision with maximum depth limit and degenerate detection
*/
void	add_curve_fill(t_fill_data *fill, t_contour *contour,
		t_curve_params params)
{
	const float	res = quad_bezier_res(params);
	float		t;
	t_vec2		a;
	t_vec2		b;

	a = params.start_pt;
	t = 0.0f;
	while (t < 1)
	{
		b = quad_bezier_pt(params.start_pt, params.ctrl_pt, params.end_pt, t);
		debug_curve_subdivisions(contour, a, b);
		add_edge(fill, a, b);
		a = b;
		t += res;
	}
	add_edge(fill, a, params.end_pt);
}
