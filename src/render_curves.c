/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_curves.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:27:24 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/26 18:43:48 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

/**
 * @brief Draw curve sequence starting from an on-curve point
 */
void	draw_curve_from_on_curve(t_env *env, t_glyf_table *glyph, int curr_idx,
		int ctr_start_idx, int ctr_end_idx, t_vec2 pos, int color,
		t_glyf_component *transform)
{
	t_vec2	next_control;
	t_vec2	screen_start;
	t_vec2	screen_control;
	t_vec2	screen_end;

	int next_idx;
	t_vec2 start_pt, end_pt, control_pt, implied_pt;
	start_pt = new_vec2(glyph->x_coordinates[curr_idx],
			glyph->y_coordinates[curr_idx]);
	start_pt = apply_transform(start_pt, transform);
	// Move to first off-curve point
	curr_idx = (curr_idx == ctr_end_idx) ? ctr_start_idx : curr_idx
		+ 1;
	while (!(glyph->flags[curr_idx] & ON_CURVE))
	{
		control_pt = new_vec2(glyph->x_coordinates[curr_idx],
				glyph->y_coordinates[curr_idx]);
		control_pt = apply_transform(control_pt, transform);
		next_idx = (curr_idx == ctr_end_idx) ? ctr_start_idx : curr_idx + 1;
		if (glyph->flags[next_idx] & ON_CURVE)
		{
			// Next point is on-curve: draw curve to it
			end_pt = new_vec2(glyph->x_coordinates[next_idx],
					glyph->y_coordinates[next_idx]);
			end_pt = apply_transform(end_pt, transform);
		}
		else
		{
			// Next point is also off-curve: create implied on-curve point
			next_control = new_vec2(glyph->x_coordinates[next_idx],
					glyph->y_coordinates[next_idx]);
			next_control = apply_transform(next_control, transform);
			implied_pt.x = (control_pt.x + next_control.x) / 2;
			implied_pt.y = (control_pt.y + next_control.y) / 2;
			end_pt = implied_pt;
		}
		// Draw the quadratic curve
		screen_start = transform_coordinate(env, start_pt, pos);
		screen_control = transform_coordinate(env, control_pt, pos);
		screen_end = transform_coordinate(env, end_pt, pos);
		ft_mlx_draw_quadratic_curve(&env->mlx->img, screen_start,
			screen_control, screen_end, color);
		// Move to next segment
		start_pt = end_pt;
		curr_idx = next_idx;
		// If we've reached an on-curve point, we're done
		if (glyph->flags[curr_idx] & ON_CURVE)
			break ;
	}
}

/**
 * @brief Simplified curve sequence function (kept for compatibility)
 */
void	draw_curve_sequence(t_env *env, t_glyf_table *glyph, int start_idx,
		int end_idx, int ctr_start_idx, int ctr_end_idx, t_vec2 pos, int color,
		t_glyf_component *transform)
{
	(void)end_idx;
	int	prev_on_curve;

	// Find the previous on-curve point
	prev_on_curve = start_idx - 1;
	if (prev_on_curve < ctr_start_idx)
		prev_on_curve = ctr_end_idx;
	while (!(glyph->flags[prev_on_curve] & ON_CURVE)
		&& prev_on_curve != start_idx)
	{
		prev_on_curve--;
		if (prev_on_curve < ctr_start_idx)
			prev_on_curve = ctr_end_idx;
	}
	draw_curve_from_on_curve(env, glyph, prev_on_curve, ctr_start_idx,
		ctr_end_idx, pos, color, transform);
}
