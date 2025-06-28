/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_contour_process.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 21:53:00 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/25 19:59:01 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

/*
	Find end point for curve segment (handles implied points)
*/
static t_vec2	find_curve_end_point(t_contour *contour, int ctrl_idx,
		t_curve_params *params)
{
	int		next_idx;
	t_vec2	next_pt;
	t_vec2	ctrl_pt;

	next_idx = ctrl_idx + 1;
	if (ctrl_idx == params->contour_end)
		next_idx = params->contour_start;
	next_pt = new_vec2(contour->glyf->x_coordinates[next_idx],
			contour->glyf->y_coordinates[next_idx]);
	if (contour->glyf->flags[next_idx] & ON_CURVE)
		return (next_pt);
	ctrl_pt = new_vec2(contour->glyf->x_coordinates[ctrl_idx],
			contour->glyf->y_coordinates[ctrl_idx]);
	return (create_implied_point(ctrl_pt, next_pt));
}

/*
	Process complete curve sequence from on-curve point
*/
static void	process_curve_sequence(t_fill_data *fill, t_contour *contour,
		t_curve_params *params)
{
	int	curr_idx;
	int	next_idx;

	curr_idx = params->contour_idx;
	params->start_pt = new_vec2(contour->glyf->x_coordinates[curr_idx],
			contour->glyf->y_coordinates[curr_idx]);
	while (1)
	{
		next_idx = curr_idx + 1;
		if (curr_idx == params->contour_end)
			next_idx = params->contour_start;
		if (contour->glyf->flags[next_idx] & ON_CURVE)
			break ;
		params->ctrl_pt = new_vec2(contour->glyf->x_coordinates[next_idx],
				contour->glyf->y_coordinates[next_idx]);
		params->end_pt = find_curve_end_point(contour, next_idx, params);
		add_curve_fill(fill, contour, *params, 0);
		params->start_pt = params->end_pt;
		curr_idx = next_idx;
	}
}

/*
	Process contour point
*/
void	process_fill_contour_point(t_fill_data *fill, t_contour *contour,
		t_curve_params *params)
{
	int		next_idx;
	t_vec2	start_pt;
	t_vec2	end_pt;

	if (!(contour->glyf->flags[params->contour_idx] & ON_CURVE))
		return ;
	next_idx = params->contour_idx + 1;
	if (params->contour_idx == params->contour_end)
		next_idx = params->contour_start;
	start_pt = new_vec2(contour->glyf->x_coordinates[params->contour_idx],
			contour->glyf->y_coordinates[params->contour_idx]);
	if (contour->glyf->flags[next_idx] & ON_CURVE)
	{
		end_pt = new_vec2(contour->glyf->x_coordinates[next_idx],
				contour->glyf->y_coordinates[next_idx]);
		add_edge(fill, start_pt, end_pt);
	}
	else
		process_curve_sequence(fill, contour, params);
}
