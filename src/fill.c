/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 21:52:49 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/14 01:38:57 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

void	fill_scanline_process(
			t_fill_data *fill);
int		get_contour_winding_direction(t_contour *contour,
			t_curve_params params);
void	process_all_off_curve_contour(t_fill_data *fill,
			t_contour *contour, t_curve_params *params);
void	cleanup_fill_data(t_fill_data *fill);
void	process_fill_contour_point(t_fill_data *fill,
			t_contour *contour, t_curve_params *params);

/*
	Initialize fill data using glyph's bounding box
*/
static t_fill_data	init_fill_data(t_contour *contour)
{
	t_fill_data	fill;
	t_vec2		min_screen;
	t_vec2		max_screen;

	min_screen = new_screen_pt(contour, contour->glyf->header->x_min,
			contour->glyf->header->y_min);
	max_screen = new_screen_pt(contour, contour->glyf->header->x_max,
			contour->glyf->header->y_max);
	if (min_screen.y < max_screen.y)
		fill.y_min = min_screen.y;
	else
		fill.y_min = max_screen.y;
	if (min_screen.y > max_screen.y)
		fill.y_max = min_screen.y;
	else
		fill.y_max = max_screen.y;
	fill.height = fill.y_max - fill.y_min + 1;
	fill.edge_table = ft_calloc(sizeof(t_edge *), fill.height);
	fill.active_edges = NULL;
	fill.env = contour->env;
	fill.color = contour->color;
	return (fill);
}

/*
	Function that checks if the contour has ON_CURVE points
*/
static bool	contour_has_on_curve(t_contour *contour, t_curve_params params)
{
	int	curr_idx;

	curr_idx = params.contour_start;
	while (curr_idx <= params.contour_end)
	{
		if (contour->glyf->flags[curr_idx] & ON_CURVE)
			return (true);
		curr_idx++;
	}
	return (false);
}

static void	process_single_contour(t_contour *contour, t_fill_data *fill,
		int contour_idx)
{
	t_curve_params	params;

	params.contour_start = 0;
	if (contour_idx != 0)
		params.contour_start = contour->glyf->end_pts[contour_idx - 1] + 1;
	params.contour_end = contour->glyf->end_pts[contour_idx];
	params.contour_direction = get_contour_winding_direction(contour, params);
	if (contour_has_on_curve(contour, params))
	{
		params.contour_idx = params.contour_start;
		while (params.contour_idx <= params.contour_end)
		{
			process_fill_contour_point(fill, contour, &params);
			params.contour_idx++;
		}
	}
	else
		process_all_off_curve_contour(fill, contour, &params);
}

/*
	Main glyph filling processing function
*/
void	fill_glyph(t_contour *contour)
{
	int			contour_idx;
	t_fill_data	fill;

	fill = init_fill_data(contour);
	if (!fill.edge_table || (fill.height <= 0))
		return ;
	contour_idx = 0;
	while (contour_idx < contour->glyf->header->number_of_contours)
	{
		process_single_contour(contour, &fill, contour_idx);
		contour_idx++;
	}
	fill_scanline_process(&fill);
	cleanup_fill_data(&fill);
}
