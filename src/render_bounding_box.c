/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_bounding_box.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:45:29 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/26 15:07:05 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

/**
 * @brief Calculate transformed bounding box
 */
static void get_transformed_bounds(t_glyf_header *header, t_glyf_component *comp,
                                  t_vec2 *min, t_vec2 *max)
{
    t_vec2 corners[4];
    t_vec2 transformed;
    int i;

    if (!comp || !(comp->flags & (HAS_SCALE | HAS_XY_SCALE | HAS_2X2_MATRIX)))
    {
        *min = new_vec2(header->x_min, header->y_min);
        *max = new_vec2(header->x_max, header->y_max);
        return;
    }

    // Get all four corners of original bounding box
    corners[0] = new_vec2(header->x_min, header->y_min);
    corners[1] = new_vec2(header->x_max, header->y_min);
    corners[2] = new_vec2(header->x_min, header->y_max);
    corners[3] = new_vec2(header->x_max, header->y_max);

    // Transform all corners and find new bounds
    *min = new_vec2(32767, 32767);
    *max = new_vec2(-32768, -32768);

    i = 0;
    while (i < 4)
    {
        transformed = apply_transform(corners[i], comp);
        if (transformed.x < min->x) min->x = transformed.x;
        if (transformed.y < min->y) min->y = transformed.y;
        if (transformed.x > max->x) max->x = transformed.x;
        if (transformed.y > max->y) max->y = transformed.y;
        i++;
    }
}

/**
 * @brief Draw bounding box with optional transformation
 */
void draw_transformed_bounding_box(t_env *env, t_glyf_header *header, t_vec2 pos, int color, t_glyf_component *comp)
{
    t_vec2 min, max;
    t_vec2 lb, rb, lt, rt;

    get_transformed_bounds(header, comp, &min, &max);

    lb = transform_coordinate(env, min, pos);
    rt = transform_coordinate(env, max, pos);
    rb = transform_coordinate(env, new_vec2(max.x, min.y), pos);
    lt = transform_coordinate(env, new_vec2(min.x, max.y), pos);

    ft_mlx_line_put(&env->mlx->img, lb, rb, color);
    ft_mlx_line_put(&env->mlx->img, lb, lt, color);
    ft_mlx_line_put(&env->mlx->img, rt, rb, color);
    ft_mlx_line_put(&env->mlx->img, rt, lt, color);
}

/**
 * @brief Draw the glyph-specific bounding box
 */
/*
void	draw_glyph_bounding_box(t_env *env, size_t glyph_index,
						t_vec2 pos, int color)
{
	t_glyf_table	*glyph;
	t_vec2			lb;
	t_vec2			rb;
	t_vec2			lt;
	t_vec2			rt;

	glyph = env->font->glyfs[glyph_index];
	if (!glyph)
		return ;

	lb = transform_coordinate(env, new_vec2(glyph->header->x_min,
						glyph->header->y_min), pos);
	rt = transform_coordinate(env, new_vec2(glyph->header->x_max,
						glyph->header->y_max), pos);
	rb = transform_coordinate(env, new_vec2(glyph->header->x_max,
						glyph->header->y_min), pos);
	lt = transform_coordinate(env, new_vec2(glyph->header->x_min,
						glyph->header->y_max), pos);

	ft_mlx_line_put(&env->mlx->img, lb, rb, color);
	ft_mlx_line_put(&env->mlx->img, lb, lt, color);
	ft_mlx_line_put(&env->mlx->img, rt, rb, color);
	ft_mlx_line_put(&env->mlx->img, rt, lt, color);
}
*/

/**
 * @brief Draw the font-wide max bounding box
 */
void	draw_max_bounding_box(t_env *env, t_vec2 pos, int color)
{
	t_vec2	lb;
	t_vec2	rb;
	t_vec2	lt;
	t_vec2	rt;
	t_head_table	*head;

	head = env->font->head;
	lb = transform_coordinate(env, new_vec2(head->x_min, head->y_min), pos);
	rt = transform_coordinate(env, new_vec2(head->x_max, head->y_max), pos);
	rb = transform_coordinate(env, new_vec2(head->x_max, head->y_min), pos);
	lt = transform_coordinate(env, new_vec2(head->x_min, head->y_max), pos);

	ft_mlx_line_put(&env->mlx->img, lb, rb, color);
	ft_mlx_line_put(&env->mlx->img, lb, lt, color);
	ft_mlx_line_put(&env->mlx->img, rt, rb, color);
	ft_mlx_line_put(&env->mlx->img, rt, lt, color);
}
