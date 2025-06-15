/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text_rendering.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 22:00:00 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/15 22:17:09 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

void	draw_glyph(t_contour *contour);

static t_vec2	transform_metrics(t_env *env, t_vec2 metrics_vec)
{
	float	scale;
	t_vec2	result;

	if (env->font->head->units_per_em == 0)
		return (new_vec2(0, 0));
	scale = 500.0f / (float)env->font->head->units_per_em;
	result.x = metrics_vec.x * scale;
	result.y = metrics_vec.y * scale;
	return (result);
}

/**
 * @brief Get glyph metrics from hmtx table
 */
t_text_metrics	get_glyph_metrics(t_env *env, size_t glyph_idx)
{
	t_text_metrics	metrics;
	t_hmtx_table	*hmtx;
	t_glyf_table	*glyph;

	ft_bzero(&metrics, sizeof(t_text_metrics));
	hmtx = env->font->hmtx;
	if (!hmtx || glyph_idx >= env->font->maxp->num_glyphs)
		return (metrics);
	if (glyph_idx < hmtx->num_lhmtx)
	{
		metrics.advance_width = hmtx->lhmtx[glyph_idx].advance_width;
		metrics.left_side_bearing = hmtx->lhmtx[glyph_idx].lsb;
	}
	else
	{
		metrics.advance_width = hmtx->lhmtx[hmtx->num_lhmtx - 1].advance_width;
		if (glyph_idx - hmtx->num_lhmtx < hmtx->num_lsbs)
			metrics.left_side_bearing = hmtx->lsbs[glyph_idx - hmtx->num_lhmtx];
		else
			metrics.left_side_bearing = hmtx->lhmtx[hmtx->num_lhmtx - 1].lsb;
	}
	glyph = env->font->glyfs[glyph_idx];
	if (glyph && glyph->header)
	{
		metrics.glyph_bbox_min = new_vec2(glyph->header->x_min,
				glyph->header->y_min);
		metrics.glyph_bbox_max = new_vec2(glyph->header->x_max,
				glyph->header->y_max);
	}
	return (metrics);
}

/**
 * @brief Calculate pen position with left side bearing
 */
static t_vec2	calculate_glyph_position(t_vec2 pen_pos, t_text_metrics metrics,
	t_env *env)
{
	t_vec2	lsb_vec;
	t_vec2	transformed_lsb;
	t_vec2	glyph_pos;

	lsb_vec = new_vec2(metrics.left_side_bearing, 0);
	transformed_lsb = transform_metrics(env, lsb_vec);  // Use metrics transform

	glyph_pos.x = pen_pos.x + transformed_lsb.x;
	glyph_pos.y = pen_pos.y;
	return (glyph_pos);
}

/**
 * @brief Advance pen position using same transform as glyph coordinates
 */
static t_vec2	advance_pen_position(t_vec2 pen_pos, t_text_metrics metrics,
	t_env *env)
{
	t_vec2	advance_vec;
	t_vec2	transformed_advance;
	t_vec2	new_pos;

	advance_vec = new_vec2(metrics.advance_width, 0);
	// Use metrics transform instead of coordinate transform
	transformed_advance = transform_metrics(env, advance_vec);

	new_pos.x = pen_pos.x + transformed_advance.x;
	new_pos.y = pen_pos.y;

	return (new_pos);
}


/**
 * @brief Draw a single character at specified position
 */
static t_vec2	draw_character(t_env *env, char c, t_vec2 pen_pos, int color)
{
	t_contour		contour;
	size_t			glyph_idx;
	t_text_metrics	metrics;
	t_vec2			glyph_pos;

	glyph_idx = get_glyph_index(env->font, c);
	if (glyph_idx >= env->font->maxp->num_glyphs)
		return (pen_pos);
	metrics = get_glyph_metrics(env, glyph_idx);
	glyph_pos = calculate_glyph_position(pen_pos, metrics, env);
	ft_bzero(&contour, sizeof(t_contour));
	contour.env = env;
	contour.color = color;
	contour.pos = glyph_pos;
	contour.glyf_idx = glyph_idx;
	contour.glyf = env->font->glyfs[glyph_idx];
	if (contour.glyf)
		draw_glyph(&contour);
	return (advance_pen_position(pen_pos, metrics, env));
}

/**
 * @brief Draw a complete string with proper spacing
 */
void	draw_string(t_env *env, const char *str, t_vec2 pos, int color)
{
	t_vec2	pen_pos;
	int		i;

	if (!str || !env || !env->font)
		return ;
	pen_pos = pos;
	i = 0;
	while (str[i])
	{
		pen_pos = draw_character(env, str[i], pen_pos, color);
		i++;
	}
}

int	measure_string_width(t_env *env, const char *str)
{
	int				total_width;
	int				i;
	size_t			glyph_idx;
	t_text_metrics	metrics;
	t_vec2			advance_vec;
	t_vec2			transformed_advance;

	if (!str || !env || !env->font || (env->font->head->units_per_em == 0))
		return (0);
	total_width = 0;
	i = 0;
	while (str[i])
	{
		glyph_idx = get_glyph_index(env->font, str[i]);
		if (glyph_idx < env->font->maxp->num_glyphs)
		{
			metrics = get_glyph_metrics(env, glyph_idx);
			advance_vec = new_vec2(metrics.advance_width, 0);
			transformed_advance = transform_metrics(env, advance_vec);
			total_width += (int)transformed_advance.x;
		}
		i++;
	}
	return (total_width);
}