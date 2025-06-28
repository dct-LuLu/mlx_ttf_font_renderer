/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_curve.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 00:12:04 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/28 17:02:31 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

static void	compute_implied_start_point(t_curve_params *params,
		t_contour *contour, t_vec2 last_ctrl)
{
	t_vec2	prev_ctrl;

	if (params->contour_idx == params->contour_start)
		params->start_pt = create_implied_point(last_ctrl, params->ctrl_pt);
	else
	{
		prev_ctrl = new_vec2(contour->glyf->x_coordinates[params->contour_idx - 1],
				contour->glyf->y_coordinates[params->contour_idx - 1]);
		if (contour->env->subpixel)
			subpixelize(&prev_ctrl);
		params->start_pt = create_implied_point(prev_ctrl, params->ctrl_pt);
	}
}

static void	compute_implied_end_point(t_curve_params *params,
		t_contour *contour, t_vec2 first_ctrl)
{
	t_vec2	next_ctrl;

	if (params->contour_idx == params->contour_end)
		params->end_pt = create_implied_point(params->ctrl_pt, first_ctrl);
	else
	{
		next_ctrl = new_vec2(
				contour->glyf->x_coordinates[params->contour_idx + 1],
				contour->glyf->y_coordinates[params->contour_idx + 1]);
		if (contour->env->subpixel)
			subpixelize(&next_ctrl);
		params->end_pt = create_implied_point(params->ctrl_pt, next_ctrl);
	}
}

/*
Handle contours with no on-curve points (all ctrl points)
*/
void	process_all_off_curve_contour(t_fill_data *fill, t_contour *contour,
		t_curve_params *params)
{
	t_vec2	first_ctrl;
	t_vec2	last_ctrl;

	if (params->contour_start == params->contour_end)
		return ;
	first_ctrl = new_vec2(
			contour->glyf->x_coordinates[params->contour_start],
			contour->glyf->y_coordinates[params->contour_start]);
	if (fill->env->subpixel)
		subpixelize(&first_ctrl);
	last_ctrl = new_vec2(
			contour->glyf->x_coordinates[params->contour_end],
			contour->glyf->y_coordinates[params->contour_end]);
	if (fill->env->subpixel)
		subpixelize(&last_ctrl);
	params->contour_idx = params->contour_start;
	while (params->contour_idx <= params->contour_end)
	{
		params->ctrl_pt = new_vec2(
				contour->glyf->x_coordinates[params->contour_idx],
				contour->glyf->y_coordinates[params->contour_idx]);
		if (fill->env->subpixel)
			subpixelize(&params->ctrl_pt);
		compute_implied_start_point(params, contour, last_ctrl);
		compute_implied_end_point(params, contour, first_ctrl);
		add_curve_fill(fill, contour, *params, 0);
		params->contour_idx++;
	}
}
