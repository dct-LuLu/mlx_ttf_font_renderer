/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_glyph.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 16:51:42 by jaubry--          #+#    #+#             */
/*   Updated: 2025/10/29 00:32:57 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"
#include "rasterizer.h"

void	draw_contour(t_contour *contour);
void	fill_glyph(t_contour *contour);

/**
 * @brief Draw a simple glyph with optional transformation
 */
static void	draw_simple_glyph(t_contour *contour)
{
	if (!contour->glyf || (contour->glyf->header->number_of_contours <= 0))
		return ;
	fill_glyph(contour);
	if (DEBUG || contour->text->outlined)
	{
		contour->idx = 0;
		if (DEBUG)
			contour->text->outline = (t_rgba_int){.rgba=RED};
		while (contour->idx < contour->glyf->header->number_of_contours)
		{
			draw_contour(contour);
			contour->idx++;
		}
		if (DEBUG)
			draw_transformed_bounding_box(contour, (t_rgba_int){.rgba=YELLOW});
	}
}

/**
 * @brief Calculate component position with offsets and scaling
 */
static t_vec2i	get_component_position(t_contour *contour, t_glyf_component *comp)
{
	t_vec2i	comp_pos;
	t_vec2i	offset;

	comp_pos = contour->pos;
	if (!comp || !(comp->flags & ARGS_ARE_XY_VALUES))
		return (comp_pos);
	offset = vec2i(comp->arg1, comp->arg2);
	if (comp->flags & SCALED_COMPONENT_OFFSET)
	offset = apply_transform(offset, comp);
	return (new_screen_pt(contour, offset.x, offset.y));
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
		if (!(comp->glyph_index >= contour->text->font->maxp->num_glyphs
				|| !contour->text->font->glyfs[comp->glyph_index]))
		{
			comp_contour.glyf = contour->text->font->glyfs[comp->glyph_index];
			ft_memcpy(comp_contour.text, contour->text, sizeof(t_text));
			comp_contour.pos = get_component_position(contour, comp);
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
	t_vec2i			corners[2];
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

	if (contour->glyf_idx >= contour->text->font->maxp->num_glyphs)
		return ;
	glyph = contour->text->font->glyfs[contour->glyf_idx];
	if (!glyph)
		return ;
	contour->glyf = glyph;
	if (is_glyph_onscreen(contour))
		return ;
	if (DEBUG)
		draw_transformed_bounding_box(contour, (t_rgba_int){.rgba=GREEN});
	if (glyph->header->number_of_contours == -1)
		draw_composite_glyph(contour);
	else if (glyph->header->number_of_contours >= 0)
		draw_simple_glyph(contour);
}
