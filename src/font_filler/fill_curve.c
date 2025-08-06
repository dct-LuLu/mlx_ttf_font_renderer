/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_curve.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 00:12:04 by jaubry--          #+#    #+#             */
/*   Updated: 2025/08/06 09:07:45 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rasterizer.h"
#include "libft.h"

static void	compute_implied_start_point(t_curve_params *params,
		t_contour *contour, t_vec2i last_ctrl)
{
	t_vec2i	prev_ctrl;

	if (params->contour_idx == params->contour_start)
		params->start_pt = create_implied_point(last_ctrl, params->ctrl_pt);
	else
	{
		prev_ctrl = get_glyf_coords(contour->glyf, params->contour_idx - 1);
		params->start_pt = create_implied_point(prev_ctrl, params->ctrl_pt);
	}
}

static void	compute_implied_end_point(t_curve_params *params,
		t_contour *contour, t_vec2i first_ctrl)
{
	t_vec2i	next_ctrl;

	if (params->contour_idx == params->contour_end)
		params->end_pt = create_implied_point(params->ctrl_pt, first_ctrl);
	else
	{
		next_ctrl = get_glyf_coords(contour->glyf, params->contour_idx + 1);
		params->end_pt = create_implied_point(params->ctrl_pt, next_ctrl);
	}
}

/*
Handle contours with no on-curve points (all ctrl points)
*/
void	process_all_off_curve_contour(t_fill_data *fill, t_contour *contour,
		t_curve_params *params)
{
	t_vec2i	first_ctrl;
	t_vec2i	last_ctrl;

	(void)fill;
	if (params->contour_start == params->contour_end)
		return ;
	first_ctrl = get_glyf_coords(contour->glyf, params->contour_start);
	last_ctrl = get_glyf_coords(contour->glyf, params->contour_end);
	params->contour_idx = params->contour_start;
	while (params->contour_idx <= params->contour_end)
	{
		params->ctrl_pt = get_glyf_coords(contour->glyf, params->contour_idx);
		compute_implied_start_point(params, contour, last_ctrl);
		compute_implied_end_point(params, contour, first_ctrl);
		add_curve_fill(fill, contour, *params);
		params->contour_idx++;
	}
}
