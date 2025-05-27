/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_glyph.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:49:33 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/26 18:42:54 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

/**
 * @brief Draw a single contour with optional transformation
 */
static void draw_contour(t_env *env, t_glyf_table *glyph, int contour_idx,
                       t_vec2 pos, int color, t_glyf_component *transform)
{
	int	curr_idx, start_idx, end_idx, next_idx;
    t_vec2 curr_pt, next_pt;
    
    start_idx = (contour_idx == 0) ? 0 : glyph->end_pts[contour_idx - 1] + 1;
    end_idx = glyph->end_pts[contour_idx];
    
    curr_idx = start_idx;
    while (curr_idx <= end_idx)
    {
        next_idx = (curr_idx == end_idx) ? start_idx : curr_idx + 1;
        curr_pt = new_vec2(glyph->x_coordinates[curr_idx],
                          glyph->y_coordinates[curr_idx]);   
        curr_pt = apply_transform(curr_pt, transform);
    
       	if (glyph->flags[curr_idx] & ON_CURVE)
       	{
       		if (glyph->flags[next_idx] & ON_CURVE) // straight line between two on-curve points
       		{
				next_pt = new_vec2(glyph->x_coordinates[next_idx],
					glyph->y_coordinates[next_idx]);
				next_pt = apply_transform(next_pt, transform);
        		next_pt = transform_coordinate(env, next_pt, pos);
				curr_pt = transform_coordinate(env, curr_pt, pos);
        		ft_mlx_line_put(&env->mlx->img, curr_pt, next_pt, color);
        	}
        	else
				draw_curve_from_on_curve(env, glyph, curr_idx, start_idx, end_idx, pos, color, transform);
        }
        curr_idx++;
    }
}

/**
 * @brief Draw a simple glyph with optional transformation
 */
static void draw_simple_glyph(t_env *env, t_glyf_table *glyph, t_vec2 pos,
                            int color, t_glyf_component *transform)
{
    int contour_index;
    
    if (!glyph || glyph->header->number_of_contours <= 0)
		return ;

    contour_index = 0;
    while (contour_index < glyph->header->number_of_contours)
    {
        draw_contour(env, glyph, contour_index, pos, color, transform);
        contour_index++;
    }
	if (DEBUG)
		draw_transformed_bounding_box(env, glyph->header, pos, YELLOW, transform);
}

/**
 * @brief Draw a composite glyph by rendering all components
 */
static void draw_composite_glyph(t_env *env, t_glyf_table *glyph, t_vec2 pos,
                               int color)
{
    t_glyf_component	*comp;
    t_glyf_table		*comp_glyph;
    t_vec2				comp_pos;
    
	if (!glyph || !glyph->component)
		return ;
    
    comp = glyph->component;
    while (comp)
    {
        if (comp->glyph_index >= env->font->maxp->num_glyphs ||
            !env->font->glyfs[comp->glyph_index])
        {
            comp = comp->next;
            continue;
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
void draw_glyph_outline(t_env *env, size_t glyph_index, t_vec2 pos, int color)
{
    t_glyf_table	*glyph;
    
	if (glyph_index >= env->font->maxp->num_glyphs)
		return ;
    
	glyph = env->font->glyfs[glyph_index];
	if (!glyph)
		return ;
    if (DEBUG && (glyph->header->number_of_contours >= -1))
		draw_transformed_bounding_box(env, glyph->header, pos, GREEN, NULL);
    if (glyph->header->number_of_contours >= 0)
        draw_simple_glyph(env, glyph, pos, color, NULL);
    else if (glyph->header->number_of_contours == -1)
        draw_composite_glyph(env, glyph, pos, color);
}

/*
int	draw_glyph_outline(t_env *env, size_t glyph_index, t_vec2 pos, int color)
{
	t_glyf_table	*glyph;
	int		contour_index;
	int		curr_idx;
	int		start_idx;
	int		end_pt;

	glyph = env->font->glyfs[glyph_index];
	if (!glyph)
		return (0);
	if (glyph->header->number_of_contours == -1)
		return (draw_composite_glyph_outline(env, glyph, pos, color));

	start_idx = 0;
	contour_index = 0;
	while (contour_index < glyph->header->number_of_contours)
	{
		end_pt = glyph->end_pts[contour_index];
		curr_idx = start_idx;
		while (curr_idx <= end_pt)
		{
			int next_idx = (curr_idx == end_pt) ? start_idx : curr_idx + 1;
			t_vec2 curr_pt = new_vec2(glyph->x_coordinates[curr_idx],
							glyph->y_coordinates[curr_idx]);
			t_vec2 next_pt = new_vec2(glyph->x_coordinates[next_idx],
							glyph->y_coordinates[next_idx]);

			ft_mlx_line_put(&env->mlx->img,
					transform_coordinate(env, curr_pt, pos),
					transform_coordinate(env, next_pt, pos), color);
			ioint_index++;
		}
		start_idx = end_pt + 1;
		contour_index++;
	}
	return (1);
}
*/
