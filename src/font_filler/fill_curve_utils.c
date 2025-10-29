/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_curve_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 21:37:58 by jaubry--          #+#    #+#             */
/*   Updated: 2025/10/29 04:52:41 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"
#include "rasterizer.h"
#include "libft.h"

static void	debug_curve_subdivisions(t_contour *contour, t_vec2i _a, t_vec2i _b)
{
	t_vec2i	a;
	t_vec2i	b;

	a = to_screen_pt(contour->text, _a, contour->pos);
	b = to_screen_pt(contour->text, _b, contour->pos);
	if (DEBUG && (contour->text->size >= 50))
	{
		ft_mlx_circle_put(contour->text->img, a, 7, (t_rgb_int){.rgb=GREEN});
		ft_mlx_circle_put(contour->text->img, b, 7, (t_rgb_int){.rgb=GREEN});
		ft_mlx_line_put(contour->text->img, a, b, (t_rgb_int){.rgb=GREEN});
	}
}

/*
	Recursive curve subdivision with maximum depth limit and degenerate detection
*/
void	add_curve_fill(t_fill_data *fill, t_contour *contour,
		t_curve_params params)
{
	const float	res = quad_bezier_res(params.start_pt,
			params.ctrl_pt, params.end_pt);
	float		t;
	t_vec2i		a;
	t_vec2i		b;

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
