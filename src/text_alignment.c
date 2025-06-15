/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text_alignment.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 22:00:00 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/15 21:47:50 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

/**
 * @brief Draw string centered at position
 */
void	draw_string_centered(t_env *env, const char *str, t_vec2 center_pos,
			int color)
{
	int		string_width;
	t_vec2	start_pos;

	string_width = measure_string_width(env, str);
	start_pos.x = center_pos.x - (string_width / 2);
	start_pos.y = center_pos.y;
	draw_string(env, str, start_pos, color);
}

/**
 * @brief Draw string right-aligned at position
 */
void	draw_string_right_aligned(t_env *env, const char *str, t_vec2 right_pos,
			int color)
{
	int		string_width;
	t_vec2	start_pos;

	string_width = measure_string_width(env, str);
	start_pos.x = right_pos.x - string_width;
	start_pos.y = right_pos.y;
	draw_string(env, str, start_pos, color);
}

/**
 * @brief Get character index at pixel position (for text editing)
 */
int	get_character_index_at_position(t_env *env, const char *str,
		t_vec2 click_pos, t_vec2 string_pos)
{
	int				i;
	int				current_width;
	int				char_width;
	size_t			glyph_idx;
	t_text_metrics	metrics;
	float			scale;

	if (!str || !env || !env->font || env->font->head->units_per_em == 0)
		return (0);
	scale = 500.0f / (float)env->font->head->units_per_em;
	current_width = 0;
	i = 0;
	while (str[i])
	{
		glyph_idx = get_glyph_index(env->font, str[i]);
		if (glyph_idx < env->font->maxp->num_glyphs)
		{
			metrics = get_glyph_metrics(env, glyph_idx);
			char_width = (int)(metrics.advance_width * scale);
			if (click_pos.x < string_pos.x + current_width + (char_width / 2))
				return (i);
			current_width += char_width;
		}
		i++;
	}
	return (i);
}
