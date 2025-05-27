/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_glyph.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 16:51:42 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/27 23:10:36 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

/**
 * @brief Initialize curve parameters structure
 */
static t_curve_params	init_curve_params(t_vec2 pos, int color,
					int start_idx, int end_idx, t_glyf_component *transform)
{
	t_curve_params	params;

	params.pos = pos;
	params.color = color;
	params.contour_start = start_idx;
	params.contour_end = end_idx;
	params.transform = transform;
	return (params);
}

/**
 * @brief Draw straight line between two on-curve points
 */
static void	draw_straight_segment(t_env *env, t_glyf_table *glyph,
				int curr_idx, int next_idx, t_curve_params *params)
{
	t_vec2	curr_pt;
	t_vec2	next_pt;
	t_vec2	screen_curr;
	t_vec2	screen_next;

	curr_pt = new_vec2(glyph->x_coordinates[curr_idx],
			glyph->y_coordinates[curr_idx]);
	next_pt = new_vec2(glyph->x_coordinates[next_idx],
			glyph->y_coordinates[next_idx]);
	curr_pt = apply_transform(curr_pt, params->transform);
	next_pt = apply_transform(next_pt, params->transform);
	screen_curr = transform_coordinate(env, curr_pt, params->pos);
	screen_next = transform_coordinate(env, next_pt, params->pos);
	ft_mlx_line_put(&env->mlx->img, screen_curr, screen_next, params->color);
}

/**
 * @brief Process single point in contour
 */
static void	process_contour_point(t_env *env, t_glyf_table *glyph,
				int curr_idx, t_curve_params *params)
{
	int	next_idx;

	if (!(glyph->flags[curr_idx] & ON_CURVE))
		return ;
	next_idx = (curr_idx == params->contour_end) ?
		params->contour_start : curr_idx + 1;
	if (glyph->flags[next_idx] & ON_CURVE)
		draw_straight_segment(env, glyph, curr_idx, next_idx, params);
	else
		draw_curve_from_on_curve(env, glyph, curr_idx, params);
}

/**
 * @brief Draw a single contour with optional transformation
 */
static void	draw_contour(t_env *env, t_glyf_table *glyph, int contour_idx,
			t_vec2 pos, int color, t_glyf_component *transform)
{
	int		curr_idx;
	int		start_idx;
	int		end_idx;
	t_curve_params	params;

	start_idx = (contour_idx == 0) ? 0 : glyph->end_pts[contour_idx - 1] + 1;
	end_idx = glyph->end_pts[contour_idx];
	params = init_curve_params(pos, color, start_idx, end_idx, transform);
	curr_idx = start_idx;
	while (curr_idx <= end_idx)
	{
		process_contour_point(env, glyph, curr_idx, &params);
		curr_idx++;
	}
}

/**
 * @brief Draw a simple glyph with optional transformation
 */
static void	draw_simple_glyph(t_env *env, t_glyf_table *glyph, t_vec2 pos,
				int color, t_glyf_component *transform)
{
	int	contour_idx;

	if (!glyph || glyph->header->number_of_contours <= 0)
		return ;
	contour_idx = 0;
	while (contour_idx < glyph->header->number_of_contours)
	{
		draw_contour(env, glyph, contour_idx, pos, color, transform);
		contour_idx++;
	}
	if (DEBUG)
		draw_transformed_bounding_box(env, glyph->header, pos, YELLOW,
			transform);
}

/**
 * @brief Draw a composite glyph by rendering all components
 */
static void	draw_composite_glyph(t_env *env, t_glyf_table *glyph, t_vec2 pos,
				int color)
{
	t_glyf_component	*comp;
	t_glyf_table		*comp_glyph;
	t_vec2			comp_pos;

	if (!glyph || !glyph->component)
		return ;
	comp = glyph->component;
	while (comp)
	{
		if (comp->glyph_index >= env->font->maxp->num_glyphs ||
			!env->font->glyfs[comp->glyph_index])
		{
			comp = comp->next;
			continue ;
		}
		comp_glyph = env->font->glyfs[comp->glyph_index];
		comp_pos = get_component_position(pos, comp);
		if (comp_glyph->header->number_of_contours >= 0)
			draw_simple_glyph(env, comp_glyph, comp_pos, color, comp);
		else if (comp_glyph->header->number_of_contours == -1)
			draw_glyph_outline(env, comp->glyph_index, comp_pos, color);
		comp = comp->next;
	}
}

/**
 * @brief Draw any glyph (simple or composite) with automatic detection
 */
void	draw_glyph_outline(t_env *env, size_t glyph_idx, t_vec2 pos, int color)
{
	t_glyf_table	*glyph;

	if (glyph_idx >= env->font->maxp->num_glyphs)
		return ;
	glyph = env->font->glyfs[glyph_idx];
	if (!glyph)
		return ;
	if (DEBUG && (glyph->header->number_of_contours >= -1))
		draw_transformed_bounding_box(env, glyph->header, pos, GREEN, NULL);
	if (glyph->header->number_of_contours >= 0)
		draw_simple_glyph(env, glyph, pos, color, NULL);
	else if (glyph->header->number_of_contours == -1)
		draw_composite_glyph(env, glyph, pos, color);
}
