/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 21:52:49 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/15 20:53:09 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

void	fill_scanline_process(t_fill_data *fill);
void	process_all_off_curve_contour(t_fill_data *fill,
			t_contour *contour, t_curve_params *params);
void	process_fill_contour_point(t_fill_data *fill,
			t_contour *contour, t_curve_params *params);

static void	init_y_min_max(t_contour *contour, t_fill_data *fill)
{
	t_vec2		min_screen;
	t_vec2		max_screen;

	min_screen = new_screen_pt(contour, contour->glyf->header->x_min,
			contour->glyf->header->y_min);
	max_screen = new_screen_pt(contour, contour->glyf->header->x_max,
			contour->glyf->header->y_max);
	if (min_screen.y < max_screen.y)
		fill->y_min = min_screen.y;
	else
		fill->y_min = max_screen.y;
	if (min_screen.y > max_screen.y)
		fill->y_max = min_screen.y;
	else
		fill->y_max = max_screen.y;
}

/*
	Initialize fill data using glyph's bounding box
*/
static t_fill_data	init_fill_data(t_contour *contour)
{
	t_fill_data	fill;
	int			i;

	ft_bzero(&fill, sizeof(t_fill_data));
	init_y_min_max(contour, &fill);
	fill.height = fill.y_max - fill.y_min + 1;
	if ((fill.height > MAX_GLYPH_HEIGHT) || (fill.height <= 0))
		return (fill);
	ft_bzero(fill.edge_table, MAX_GLYPH_HEIGHT);
	fill.edge_pool.free_count = MAX_ACTIVE_EDGES;
	i = 0;
	while (i < MAX_ACTIVE_EDGES)
	{
		fill.edge_pool.free_indices[i] = i;
		i++;
	}
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
	if (fill.height <= 0)
		return ;
	contour_idx = 0;
	while (contour_idx < contour->glyf->header->number_of_contours)
	{
		process_single_contour(contour, &fill, contour_idx);
		contour_idx++;
	}
	fill_scanline_process(&fill);
}
