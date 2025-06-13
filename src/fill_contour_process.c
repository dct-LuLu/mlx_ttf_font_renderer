/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_contour_process.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 21:53:00 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/13 22:32:54 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

/*
	Handle contours with no on-curve points (all control points)
*/
void	process_all_off_curve_contour(t_fill_data *fill, t_contour *contour,
		t_curve_params *params, int contour_direction)
{
	int		curr_idx;
	t_vec2	first_control;
	t_vec2	last_control;
	t_vec2	prev_control;
	t_vec2	next_control;

	if (params->contour_start == params->contour_end)
		return ;
	first_control = new_screen_pt(contour,
			contour->glyf->x_coordinates[params->contour_start],
			contour->glyf->y_coordinates[params->contour_start]);
	last_control = new_screen_pt(contour,
			contour->glyf->x_coordinates[params->contour_end],
			contour->glyf->y_coordinates[params->contour_end]);
	curr_idx = params->contour_start;
	while (curr_idx <= params->contour_end)
	{
		params->control_pt = new_screen_pt(contour,
				contour->glyf->x_coordinates[curr_idx],
				contour->glyf->y_coordinates[curr_idx]);
		// Calculate implied start point
		if (curr_idx == params->contour_start)
			params->start_pt = create_implied_point(last_control, params->control_pt);
		else
		{
			prev_control = new_screen_pt(contour,
					contour->glyf->x_coordinates[curr_idx - 1],
					contour->glyf->y_coordinates[curr_idx - 1]);
			params->start_pt = create_implied_point(prev_control, params->control_pt);
		}
		// Calculate implied end point
		if (curr_idx == params->contour_end)
			params->end_pt = create_implied_point(params->control_pt, first_control);
		else
		{
			next_control = new_screen_pt(contour,
					contour->glyf->x_coordinates[curr_idx + 1],
					contour->glyf->y_coordinates[curr_idx + 1]);
			params->end_pt = create_implied_point(params->control_pt, next_control);
		}
		add_curve_fill(fill, contour, *params, contour_direction, 0);
		curr_idx++;
	}
}

/*
	Find end point for curve segment (handles implied points)
*/
static t_vec2	find_curve_end_point(t_contour *contour, int control_idx,
		t_curve_params *params)
{
	int		next_idx;
	t_vec2	next_pt;
	t_vec2	control_pt;

	next_idx = control_idx + 1;
	if (control_idx == params->contour_end)
		next_idx = params->contour_start;
	next_pt = new_screen_pt(contour, contour->glyf->x_coordinates[next_idx],
			contour->glyf->y_coordinates[next_idx]);
	if (contour->glyf->flags[next_idx] & ON_CURVE)
	{
		// Next point is on-curve, use it directly
		return (next_pt);
	}
	else
	{
		// Next point is also off-curve, create implied on-curve point
		control_pt = new_screen_pt(contour,
				contour->glyf->x_coordinates[control_idx],
				contour->glyf->y_coordinates[control_idx]);
		return (create_implied_point(control_pt, next_pt));
	}
}

/*
	Process complete curve sequence from on-curve point
*/
static void	process_curve_sequence(t_fill_data *fill, t_contour *contour,
		int start_idx, t_curve_params *params, int contour_direction)
{
	int		curr_idx;
	int		next_idx;

	curr_idx = start_idx;
	params->start_pt = new_screen_pt(contour, contour->glyf->x_coordinates[curr_idx],
			contour->glyf->y_coordinates[curr_idx]);
	// Process curve segments until we hit another on-curve point
	while (1)
	{
		next_idx = curr_idx + 1;
		if (curr_idx == params->contour_end)
			next_idx = params->contour_start;
		// If next point is off-curve, we have a curve segment
		if (!(contour->glyf->flags[next_idx] & ON_CURVE))
		{
			params->control_pt = new_screen_pt(contour,
					contour->glyf->x_coordinates[next_idx],
					contour->glyf->y_coordinates[next_idx]);
			// Find end point (could be on-curve or implied)
			params->end_pt = find_curve_end_point(contour, next_idx, params);
			// Add this curve segment
			add_curve_fill(fill, contour, *params, contour_direction, 0);
			// Move to end point for next segment
			params->start_pt = params->end_pt;
			curr_idx = next_idx;
		}
		else
		{
			// Next point is on-curve - we're done with this sequence
			break ;
		}
	}
}

/*
	Process contour point
*/
void	process_fill_contour_point(t_fill_data *fill, t_contour *contour,
		int curr_idx, t_curve_params *params, int contour_direction)
{
	int		next_idx;
	t_vec2	start_pt;
	t_vec2	end_pt;

	if (!(contour->glyf->flags[curr_idx] & ON_CURVE))
		return ;
	next_idx = curr_idx + 1;
	if (curr_idx == params->contour_end)
		next_idx = params->contour_start;
	start_pt = new_screen_pt(contour, contour->glyf->x_coordinates[curr_idx],
			contour->glyf->y_coordinates[curr_idx]);
	if (contour->glyf->flags[next_idx] & ON_CURVE)
	{
		end_pt = new_screen_pt(contour, contour->glyf->x_coordinates[next_idx],
				contour->glyf->y_coordinates[next_idx]);
		add_edge(fill, start_pt, end_pt, contour_direction);
	}
	else
		process_curve_sequence(fill, contour, curr_idx, params,
			contour_direction);
}
