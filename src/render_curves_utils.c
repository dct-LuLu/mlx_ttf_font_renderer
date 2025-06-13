/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_curves_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:20:40 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/14 01:49:35 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

t_vec2	transform_coordinate(t_env *env, t_vec2 glyph_pos, t_vec2 base_pos);

/**
 * @brief Get next index in contour with wrap-around
 */
int	get_next_contour_idx(int curr_idx, int start_idx, int end_idx)
{
	if (curr_idx == end_idx)
		return (start_idx);
	return (curr_idx + 1);
}

/**
 * @brief Get transformed point from glyph coordinates
 */
t_vec2	get_transformed_point(t_glyf_table *glyph, int point_idx,
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
t_vec2	create_implied_point(t_vec2 ctrl1_pt, t_vec2 ctrl2_pt)
{
	t_vec2	implied_pt;

	implied_pt.x = (ctrl1_pt.x + ctrl2_pt.x) / 2;
	implied_pt.y = (ctrl1_pt.y + ctrl2_pt.y) / 2;
	return (implied_pt);
}

/**
 * @brief Draw single quadratic curve segment using a start ctrl 
 * and end point.
 */
void	draw_curve_segment(t_contour *contour, t_vec2 start_pt,
		t_vec2 ctrl_pt, t_vec2 end_pt)
{
	t_vec2	screen[3];

	screen[0] = transform_coordinate(contour->env, start_pt, contour->pos);
	screen[1] = transform_coordinate(contour->env, ctrl_pt, contour->pos);
	screen[2] = transform_coordinate(contour->env, end_pt, contour->pos);
	ft_mlx_draw_quadratic_curve(&contour->env->mlx->img, screen,
		contour->color);
	if (DEBUG)
	{
		ft_mlx_circle_put(&contour->env->mlx->img, screen[0],
			(3 / (float)contour->env->zoom), RED);
		ft_mlx_circle_put(&contour->env->mlx->img, screen[1],
			(4 / (float)contour->env->zoom), YELLOW);
		ft_mlx_circle_put(&contour->env->mlx->img, screen[2],
			(3 / (float)contour->env->zoom), RED);
	}
}

/**
 * @brief Determine end point for curve (on-curve or implied)
 */
t_vec2	get_curve_end_point(t_glyf_table *glyph, t_vec2 ctrl_pt,
		int next_idx, t_glyf_component *transform)
{
	t_vec2	next_ctrl_pt;

	if (glyph->flags[next_idx] & ON_CURVE)
		return (get_transformed_point(glyph, next_idx, transform));
	next_ctrl_pt = get_transformed_point(glyph, next_idx, transform);
	return (create_implied_point(ctrl_pt, next_ctrl_pt));
}
