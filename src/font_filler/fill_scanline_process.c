/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_scanline_process.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 21:52:45 by jaubry--          #+#    #+#             */
/*   Updated: 2025/07/17 23:20:03 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

int		count_edges(t_edge *edge);
void	move_edges_to_active(t_fill_data *fill);
void	sort_active_edges(t_edge **active_edges);
void	remove_finished_edges(t_fill_data *fill, t_edge **aet, int scanline_y);
void	increment_x_positions(t_edge *aet);
void	sort_intersections_with_windings(int *intersections, int *windings,
			int count);
int		fill_intersections_windings(t_fill_data *fill, t_edge *active_edges,
			int y);
void	render_subpixel_line(t_fill_data *fill, int *sub_x, int *x, int y);

/*
	Fill a single horizontal line subpixel antialiased or not.
*/
static void	render_fill_line(t_fill_data *fill, t_contour *contour, int *_x)
{
	float	transformed_x[2];
	int		sub_x[2];
	int		x[2];
	int		y;

	transformed_x[0] = scale_x(contour->text, contour->pos.x, _x[0]);
	transformed_x[1] = scale_x(contour->text, contour->pos.x, _x[1]);
	y = scale_y(contour->text, contour->pos.y, fill->y);
	x[0] = (int)(transformed_x[0]);
	x[1] = (int)(transformed_x[1]);
	if (contour->text->subpixel)
	{
		sub_x[0] = transformed_x[0] * 3;
		sub_x[1] = transformed_x[1] * 3;
		render_subpixel_line(fill, sub_x, x, y);
	}
	else
		ft_mlx_horizontal_line(contour->text->img, x, y, fill->text->fg);
}

/*
	Fill based on winding rule
*/
static void	fill_horizontal(t_fill_data *fill, int intersection_count,
	t_contour *contour)
{
	int	winding_number;
	int	x[2];
	int	i;

	winding_number = 0;
	x[0] = -1;
	i = 0;
	while (i < intersection_count)
	{
		winding_number += fill->windings[i];
		if ((x[0] == -1) && (winding_number != 0))
			x[0] = fill->intersections[i];
		else if ((x[0] != -1) && (winding_number == 0))
		{
			x[1] = fill->intersections[i];
			if (x[1] > x[0])
				render_fill_line(fill, contour, x);
			x[0] = -1;
		}
		i++;
	}
}

/*
	Enhanced scanline filling with proper vertex intersection rules
*/
static void	fill_scanline_horizontal(t_fill_data *fill,
	t_edge *active_edges, t_contour *contour)
{
	const int	intersection_count = fill_intersections_windings(fill,
			active_edges, fill->y);

	if (intersection_count != 0)
	{
		sort_intersections_with_windings(fill->intersections, fill->windings,
			intersection_count);
		fill_horizontal(fill, intersection_count, contour);
	}
}

void	fill_scanline_process(t_fill_data *fill, t_contour *contour)
{
	int	edges_after;

	fill->y = fill->y_min;
	while (fill->y <= fill->y_max)
	{
		move_edges_to_active(fill);
		edges_after = count_edges(fill->active_edges);
		if (edges_after > 0)
		{
			sort_active_edges(&fill->active_edges);
			fill_scanline_horizontal(fill, fill->active_edges, contour);
		}
		remove_finished_edges(fill, &fill->active_edges, fill->y);
		increment_x_positions(fill->active_edges);
		fill->y++;
	}
}
