/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_draw_curves.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:19:25 by jaubry--          #+#    #+#             */
/*   Updated: 2025/08/06 09:06:43 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "mlx_utils.h"
#include "math_utils.h"

void	ft_mlx_draw_quadratic_curve(t_img_data *img, t_vec2i *pts, int color)
{
	float	curve_length;
	float	t;
	float	step;
	t_vec2i	current;
	t_vec2i	previous;

	curve_length = quad_curve_length(pts[0], pts[1], pts[2]);
	step = 2.0f / (curve_length + 1.0f);
	if (step > 0.1f)
		step = 0.1f;
	if (step < 0.01f)
		step = 0.01f;
	previous = pts[0];
	t = step;
	while (t <= 1.0f)
	{
		current = quad_bezier_pt(pts[0], pts[1], pts[2], t);
		ft_mlx_line_put(img, previous, current, color);
		previous = current;
		t += step;
	}
	ft_mlx_line_put(img, previous, pts[2], color);
}
