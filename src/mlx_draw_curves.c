/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_draw_curves.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:19:25 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/14 00:10:58 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "mlx_utils.h"
#include <math.h>

static t_vec2	quadratic_bezier_point(t_vec2 p0, t_vec2 p1, t_vec2 p2, float t)
{
	float	m;
	t_vec2	result;

	m = 1.0f - t;
	result.x = m * m * p0.x + 2 * m * t * p1.x + t * t * p2.x;
	result.y = m * m * p0.y + 2 * m * t * p1.y + t * t * p2.y;
	return (result);
}

static float	calculate_curve_length(t_vec2 p0, t_vec2 p1, t_vec2 p2)
{
	float	chord_length;
	float	ctrl_length;

	chord_length = sqrt((p2.x - p0.x) * (p2.x - p0.x) + (p2.y - p0.y) * (p2.y
				- p0.y));
	ctrl_length = sqrt((p1.x - p0.x) * (p1.x - p0.x) + (p1.y - p0.y) * (p1.y
				- p0.y)) + sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y)
			* (p2.y - p1.y));
	return ((chord_length + ctrl_length) * 0.5f);
}

void	ft_mlx_draw_quadratic_curve(t_img *img, t_vec2 *pts, int color)
{
	float	curve_length;
	float	t;
	float	step;
	t_vec2	current;
	t_vec2	previous;

	curve_length = calculate_curve_length(pts[0], pts[1], pts[2]);
	step = 2.0f / (curve_length + 1.0f);
	if (step > 0.1f)
		step = 0.1f;
	if (step < 0.01f)
		step = 0.01f;
	previous = pts[0];
	t = step;
	while (t <= 1.0f)
	{
		current = quadratic_bezier_point(pts[0], pts[1], pts[2], t);
		ft_mlx_line_put(img, previous, current, color);
		previous = current;
		t += step;
	}
	ft_mlx_line_put(img, previous, pts[2], color);
}
