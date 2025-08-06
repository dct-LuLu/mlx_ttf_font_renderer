/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_outline.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:26:33 by jaubry--          #+#    #+#             */
/*   Updated: 2025/08/06 10:25:32 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"
#include "rasterizer.h"

void	draw_all_off_curve_contour(t_contour *contour, t_curve_params *params);
int		has_on_curve_points(t_glyf_table *glyph, int start_idx, int end_idx);
void	draw_curve_from_on_curve(t_contour *contour, int start_idx,
			t_curve_params *params);

/**
 * @brief Initialize curve parameters structure
 */
static t_curve_params	init_curve_params(int start_idx, int end_idx)
{
	t_curve_params	params;

	params.contour_start = start_idx;
	params.contour_end = end_idx;
	return (params);
}

static void	debug_draw_segments(t_contour *contour, t_vec2i *pt)
{
	if (contour->text->size <= 0)
	{
		ft_mlx_circle_put(contour->text->img, pt[0], 5, GREEN);
		ft_mlx_circle_put(contour->text->img, pt[1], 5, GREEN);
	}
	else
	{
		ft_mlx_circle_put(contour->text->img, pt[0],
			(7 / (float)contour->text->size), GREEN);
		ft_mlx_circle_put(contour->text->img, pt[1],
			(7 / (float)contour->text->size), GREEN);
	}
}

/**
 * @brief Draw straight line between two on-curve points
 */
static void	draw_straight_segment(t_contour *contour, int curr_idx,
		int next_idx)
{
	t_vec2i	pt[2];

	pt[0] = new_screen_pt(contour, contour->glyf->x_coordinates[curr_idx],
			contour->glyf->y_coordinates[curr_idx]);
	pt[1] = new_screen_pt(contour, contour->glyf->x_coordinates[next_idx],
			contour->glyf->y_coordinates[next_idx]);
	if (DEBUG)
		debug_draw_segments(contour, pt);
	else
		ft_mlx_line_put(contour->text->img, pt[0], pt[1],
			contour->text->outline);
}

/**
 * @brief Process single point in contour
 */
static void	process_contour_point(t_contour *contour, int curr_idx,
		t_curve_params *params)
{
	int	next_idx;

	if (!(contour->glyf->flags[curr_idx] & ON_CURVE))
		return ;
	next_idx = params->contour_start;
	if (curr_idx != params->contour_end)
		next_idx = curr_idx + 1;
	if (contour->glyf->flags[next_idx] & ON_CURVE)
		draw_straight_segment(contour, curr_idx, next_idx);
	else
		draw_curve_from_on_curve(contour, curr_idx, params);
}

/**
 * @brief Draw a single contour with optional transformation
 */
void	draw_contour(t_contour *contour)
{
	int				curr_idx;
	int				start_idx;
	int				end_idx;
	t_curve_params	params;

	start_idx = 0;
	if (contour->idx != 0)
		start_idx = contour->glyf->end_pts[contour->idx - 1] + 1;
	end_idx = contour->glyf->end_pts[contour->idx];
	params = init_curve_params(start_idx, end_idx);
	if (!has_on_curve_points(contour->glyf, start_idx, end_idx))
	{
		draw_all_off_curve_contour(contour, &params);
		return ;
	}
	curr_idx = start_idx;
	while (curr_idx <= end_idx)
	{
		process_contour_point(contour, curr_idx, &params);
		curr_idx++;
	}
}
