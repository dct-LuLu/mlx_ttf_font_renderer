/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_curves.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 16:41:38 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/10 20:58:11 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

/**
 * @brief Get next index in contour with wrap-around
 */
static int	get_next_contour_idx(int curr_idx, int start_idx, int end_idx)
{
	if (curr_idx == end_idx)
		return (start_idx);
	return (curr_idx + 1);
}

/**
 * @brief Get transformed point from glyph coordinates
 */
static t_vec2	get_transformed_point(t_glyf_table *glyph, int point_idx,
		t_glyf_component *transform)
{
	t_vec2	point_pt;

	point_pt = new_vec2(glyph->x_coordinates[point_idx],
			glyph->y_coordinates[point_idx]);
	return (apply_transform(point_pt, transform));
}

/**
 * @brief Create implied on-curve point between two off-curve points
 */
static t_vec2	create_implied_point(t_vec2 control1_pt, t_vec2 control2_pt)
{
	t_vec2	implied_pt;

	implied_pt.x = (control1_pt.x + control2_pt.x) / 2;
	implied_pt.y = (control1_pt.y + control2_pt.y) / 2;
	return (implied_pt);
}

/**
 * @brief Draw single quadratic curve segment
 */
static void	draw_curve_segment(t_env *env, t_vec2 start_pt, t_vec2 control_pt,
		t_vec2 end_pt, t_curve_params *params)
{
	t_vec2	screen_start;
	t_vec2	screen_control;
	t_vec2	screen_end;

	screen_start = transform_coordinate(env, start_pt, params->pos);
	screen_control = transform_coordinate(env, control_pt, params->pos);
	screen_end = transform_coordinate(env, end_pt, params->pos);
	ft_mlx_draw_quadratic_curve(&env->mlx->img, screen_start, screen_control,
		screen_end, params->color);
}

/**
 * @brief Determine end point for curve (on-curve or implied)
 */
static t_vec2	get_curve_end_point(t_glyf_table *glyph, t_vec2 control_pt,
		int next_idx, t_glyf_component *transform)
{
	t_vec2	next_control_pt;

	if (glyph->flags[next_idx] & ON_CURVE)
		return (get_transformed_point(glyph, next_idx, transform));
	next_control_pt = get_transformed_point(glyph, next_idx, transform);
	return (create_implied_point(control_pt, next_control_pt));
}

/**
 * @brief Process single curve iteration in sequence
 */
static int	process_curve_iteration(t_glyf_table *glyph, t_curve_state *state,
		t_curve_params *params, t_env *env)
{
	t_vec2	control_pt;
	t_vec2	end_pt;

	control_pt = get_transformed_point(glyph, state->curr_idx,
			params->transform);
	state->next_idx = get_next_contour_idx(state->curr_idx, state->start_idx,
			state->end_idx);
	end_pt = get_curve_end_point(glyph, control_pt, state->next_idx,
			params->transform);
	draw_curve_segment(env, state->start_pt, control_pt, end_pt, params);
	state->start_pt = end_pt;
	state->curr_idx = state->next_idx;
	return (glyph->flags[state->next_idx] & ON_CURVE);
}

/**
 * @brief Draw curve sequence starting from an on-curve point
 */
void	draw_curve_from_on_curve(t_env *env, t_glyf_table *glyph, int start_idx,
		t_curve_params *params)
{
	t_curve_state	state;

	state.curr_idx = start_idx;
	state.start_idx = params->contour_start;
	state.end_idx = params->contour_end;
	state.start_pt = get_transformed_point(glyph, state.curr_idx,
			params->transform);
	state.curr_idx = get_next_contour_idx(state.curr_idx, state.start_idx,
			state.end_idx);
	while (!(glyph->flags[state.curr_idx] & ON_CURVE))
	{
		if (process_curve_iteration(glyph, &state, params, env))
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
		control_pt = get_transformed_point(contour->glyf, curr_idx, contour->transform);
		start_pt = create_implied_point(get_transformed_point(contour->glyf,
					(curr_idx == params->contour_start) ? params->contour_end : curr_idx
					- 1, contour->transform), control_pt);
		end_pt = create_implied_point(control_pt, get_transformed_point(contour->glyf,
					(curr_idx == params->contour_end) ? params->contour_start : curr_idx
					+ 1, contour->transform));
		draw_curve_segment(contour->env, start_pt, control_pt, end_pt, params);
		curr_idx++;
	}
}
