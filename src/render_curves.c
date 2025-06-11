/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_curves.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 16:41:38 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/11 16:25:19 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

int		get_next_contour_idx(int curr_idx, int start_idx, int end_idx);
t_vec2	get_transformed_point(t_glyf_table *glyph, int point_idx,
			t_glyf_component *transform);
t_vec2	create_implied_point(t_vec2 control1_pt, t_vec2 control2_pt);
void	draw_curve_segment(t_contour *contour, t_vec2 start_pt,
			t_vec2 control_pt, t_vec2 end_pt);
t_vec2	get_curve_end_point(t_glyf_table *glyph, t_vec2 control_pt,
			int next_idx, t_glyf_component *transform);

/**
 * @brief Process single curve iteration in sequence
 */
static int	process_curve_iteration(t_contour *contour, t_curve_state *state)
{
	t_vec2	control_pt;
	t_vec2	end_pt;

	control_pt = get_transformed_point(contour->glyf, state->curr_idx,
			contour->transform);
	state->next_idx = get_next_contour_idx(state->curr_idx, state->start_idx,
			state->end_idx);
	end_pt = get_curve_end_point(contour->glyf, control_pt, state->next_idx,
			contour->transform);
	draw_curve_segment(contour, state->start_pt, control_pt, end_pt);
	state->start_pt = end_pt;
	state->curr_idx = state->next_idx;
	return (contour->glyf->flags[state->next_idx] & ON_CURVE);
}

/**
 * @brief Draw curve sequence starting from an on-curve point
 */
void	draw_curve_from_on_curve(t_contour *contour, int start_idx,
		t_curve_params *params)
{
	t_curve_state	state;

	state.curr_idx = start_idx;
	state.start_idx = params->contour_start;
	state.end_idx = params->contour_end;
	state.start_pt = get_transformed_point(contour->glyf, state.curr_idx,
			contour->transform);
	state.curr_idx = get_next_contour_idx(state.curr_idx, state.start_idx,
			state.end_idx);
	while (!(contour->glyf->flags[state.curr_idx] & ON_CURVE))
	{
		if (process_curve_iteration(contour, &state))
			break ;
	}
}

int	has_on_curve_points(t_glyf_table *glyph, int start_idx, int end_idx)
{
	int	curr_idx;

	curr_idx = start_idx;
	while (curr_idx <= end_idx)
	{
		if (glyph->flags[curr_idx] & ON_CURVE)
			return (1);
		curr_idx++;
	}
	return (0);
}

void	draw_all_off_curve_contour(t_contour *contour, t_curve_params *params)
{
	int		curr_idx;
	t_vec2	start_pt;
	t_vec2	control_pt;
	t_vec2	end_pt;

	curr_idx = params->contour_start;
	while (curr_idx <= params->contour_end)
	{
		control_pt = get_transformed_point(contour->glyf, curr_idx,
				contour->transform);
		if (curr_idx == params->contour_start)
			start_pt = create_implied_point(get_transformed_point(contour->glyf,
						params->contour_end, contour->transform), control_pt);
		else
			start_pt = create_implied_point(get_transformed_point(contour->glyf,
						curr_idx - 1, contour->transform), control_pt);
		if (curr_idx == params->contour_end)
			end_pt = create_implied_point(get_transformed_point(contour->glyf,
						params->contour_start, contour->transform), control_pt);
		else
			end_pt = create_implied_point(get_transformed_point(contour->glyf,
						curr_idx + 1, contour->transform), control_pt);
		draw_curve_segment(contour, start_pt, control_pt, end_pt);
		curr_idx++;
	}
}
