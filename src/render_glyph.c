/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_glyph.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 16:51:42 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/29 15:48:04 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

void	draw_contour(t_contour *contour);
void	fill_glyph(t_contour *contour);

/**
 * @brief Draw a simple glyph with optional transformation
 */
static void	draw_simple_glyph(t_contour *contour)
{
	int	color;

	if (!contour->glyf || (contour->glyf->header->number_of_contours <= 0))
		return ;
	fill_glyph(contour);
	if (DEBUG)
	{
		contour->idx = 0;
		color = contour->color;
		while (contour->idx < contour->glyf->header->number_of_contours)
		{
			contour->color = RED;
			draw_contour(contour);
			contour->idx++;
		}
		contour->color = color;
		draw_transformed_bounding_box(contour, YELLOW);
	}
}

/**
 * @brief Calculate component position with offsets and scaling
 */
t_vec2	get_component_position(t_vec2 base_pos, t_glyf_component *comp)
{
	t_vec2	comp_pos;
	t_vec2	offset;

	comp_pos = base_pos;
	if (!comp || !(comp->flags & ARGS_ARE_XY_VALUES))
		return (comp_pos);
	offset = new_vec2(comp->arg1, comp->arg2);
	if (comp->flags & SCALED_COMPONENT_OFFSET)
		offset = apply_transform(offset, comp);
	comp_pos.x += offset.x;
	comp_pos.y += -offset.y;
	return (comp_pos);
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
			else if (comp_contour.glyf->header->number_of_contours == -1)
				draw_composite_glyph(&comp_contour);
		}
		comp = comp->next;
	}
}

static bool	is_glyph_onscreen(t_contour *contour)
{
	t_vec2			corners[2];
	t_glyf_header	*header;

	header = contour->glyf->header;
	corners[0] = new_screen_pt(contour, header->x_min, header->y_min);
	corners[1] = new_screen_pt(contour, header->x_max, header->y_max);
	if ((corners[1].x < 0) || (corners[0].y < 0)
		|| (corners[0].x > WIDTH) || (corners[1].y > HEIGHT))
		return (true);
	return (false);
}

/*
	Draw any glyph (simple or composite)
*/
void	draw_glyph(t_contour *contour)
{
	t_glyf_table	*glyph;

	if (contour->glyf_idx >= contour->env->font->maxp->num_glyphs)
		return ;
	glyph = contour->env->font->glyfs[contour->glyf_idx];
	if (!glyph)
		return ;
	contour->glyf = glyph;
	if (is_glyph_onscreen(contour))
		return ;
	if (DEBUG)
		draw_transformed_bounding_box(contour, GREEN);
	if (glyph->header->number_of_contours == -1)
		draw_composite_glyph(contour);
	else if (glyph->header->number_of_contours >= 0)
		draw_simple_glyph(contour);
}
