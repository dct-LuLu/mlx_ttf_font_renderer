/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_glyph.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 16:51:42 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/10 20:56:26 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

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

/**
 * @brief Draw straight line between two on-curve points
 */
static void	draw_straight_segment(t_contour *contour, int curr_idx,
		int next_idx)
{
	t_vec2	curr_pt;
	t_vec2	next_pt;
	t_vec2	screen_curr;
	t_vec2	screen_next;

	curr_pt = new_vec2(contour->glyf->x_coordinates[curr_idx],
			contour->glyf->y_coordinates[curr_idx]);
	next_pt = new_vec2(contour->glyf->x_coordinates[next_idx],
			contour->glyf->y_coordinates[next_idx]);
	curr_pt = apply_transform(curr_pt, contour->transform);
	next_pt = apply_transform(next_pt, contour->transform);
	screen_curr = transform_coordinate(contour->env, curr_pt, contour->pos);
	screen_next = transform_coordinate(contour->env, next_pt, contour->pos);
	ft_mlx_line_put(&contour->env->mlx->img, screen_curr, screen_next, contour->color);
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
static void	draw_contour(t_contour *contour)
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

/**
 * @brief Draw a simple glyph with optional transformation
 */
static void	draw_simple_glyph(t_contour *contour)
{
	if (!contour->glyf || contour->glyf->header->number_of_contours <= 0)
		return ;
	contour->idx = 0;
	while (contour->idx < contour->glyf->header->number_of_contours)
	{
		draw_contour(contour);
		contour->idx++;
	}
	if (DEBUG)
		draw_transformed_bounding_box(contour, YELLOW);
}

/**
 * @brief Draw a composite glyph by rendering all components
 */
static void	draw_composite_glyph(t_contour *contour)
{
	t_glyf_component	*comp;
	t_contour			comp_contour;

	if (!contour->glyf || !contour->glyf->component)
		return ;
	comp = contour->glyf->component;
	ft_memcpy(&comp_contour, contour, sizeof(t_contour));
	while (comp)
	{
		if (!(comp->glyph_index >= contour->env->font->maxp->num_glyphs
			|| !contour->env->font->glyfs[comp->glyph_index]))
		{
			comp_contour.glyf = contour->env->font->glyfs[comp->glyph_index];
			comp_contour.pos = get_component_position(contour->pos, comp);
			comp_contour.transform = comp;
			if (comp_contour.glyf->header->number_of_contours >= 0)
				draw_simple_glyph(&comp_contour);
		}
		comp = comp->next;
	}
}

/**
 * @brief Draw any glyph (simple or composite) with automatic detection
 */
void	draw_glyph_outline(t_contour *contour)
{
	t_glyf_table	*glyph;

	if (contour->glyf_idx >= contour->env->font->maxp->num_glyphs)
		return ;
	glyph = contour->env->font->glyfs[contour->glyf_idx];
	if (!glyph)
		return ;
	contour->glyf = glyph;
	printf("glyf:%p, idx:%d, pos:%d %d, color:%d, transform:%p, env:%p\n", contour->glyf, contour->idx, contour->pos.x, contour->pos.y, contour->color, contour->transform, contour->env);
	if (DEBUG)
		draw_transformed_bounding_box(contour, GREEN);
	if (glyph->header->number_of_contours == -1)
		draw_composite_glyph(contour);
	else if (glyph->header->number_of_contours >= 0)
		draw_simple_glyph(contour);
}
