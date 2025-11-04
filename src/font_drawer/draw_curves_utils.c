/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_curves_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:20:40 by jaubry--          #+#    #+#             */
/*   Updated: 2025/11/04 18:28:00 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"
#include "rasterizer.h"

t_vec2i	to_screen_pt(t_text *text, t_vec2i glyph_pos, t_vec2i base_pos);

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
 * @brief Create implied on-curve point between two off-curve points
 */
t_vec2i	create_implied_point(t_vec2i ctrl1_pt, t_vec2i ctrl2_pt)
{
	t_vec2i	implied_pt;

	implied_pt.x = (ctrl1_pt.x + ctrl2_pt.x) / 2;
	implied_pt.y = (ctrl1_pt.y + ctrl2_pt.y) / 2;
	return (implied_pt);
}

static void	debug_quadratic_curves_info(t_contour *contour, t_vec2i *screen)
{
	const t_rgb_int	red = (t_rgb_int){.rgb=RED};
	const t_rgb_int	yellow = (t_rgb_int){.rgb=YELLOW};

	if (contour->text->font_size >= 50)
	{
		ft_mlx_safe_circle_put(contour->text->img, screen[0], 4, red);
		ft_mlx_safe_circle_put(contour->text->img, screen[1], 5, yellow);
		ft_mlx_safe_circle_put(contour->text->img, screen[2], 4, red);
	}
	else
	{
		ft_mlx_safe_circle_put(contour->text->img, screen[0],
			(3 / (float)contour->text->font_size), red);
		ft_mlx_safe_circle_put(contour->text->img, screen[1],
			(4 / (float)contour->text->font_size), yellow);
		ft_mlx_safe_circle_put(contour->text->img, screen[2],
			(3 / (float)contour->text->font_size), red);
	}
}

/**
 * @brief Draw single quadratic curve segment using a start ctrl 
 * and end point.
 */
void	draw_curve_segment(t_contour *contour, t_vec2i start_pt,
		t_vec2i ctrl_pt, t_vec2i end_pt)
{
	t_vec2i	screen[3];

	screen[0] = to_screen_pt(contour->text, start_pt, contour->pos);
	screen[1] = to_screen_pt(contour->text, ctrl_pt, contour->pos);
	screen[2] = to_screen_pt(contour->text, end_pt, contour->pos);
	ft_mlx_quad_curve_aput(contour->text->img, screen,
		contour->text->outline);
	if (DEBUG)
		debug_quadratic_curves_info(contour, screen);
}

/**
 * @brief Determine end point for curve (on-curve or implied)
 */
t_vec2i	get_curve_end_point(t_glyf_table *glyph, t_vec2i ctrl_pt,
		int next_idx, t_glyf_component *transform)
{
	t_vec2i	next_ctrl_pt;

	if (glyph->flags[next_idx] & ON_CURVE)
		return (get_transformed_point(glyph, next_idx, transform));
	next_ctrl_pt = get_transformed_point(glyph, next_idx, transform);
	return (create_implied_point(ctrl_pt, next_ctrl_pt));
}
