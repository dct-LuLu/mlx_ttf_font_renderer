/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_scanline_process.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 21:52:45 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/30 14:09:21 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

int			count_edges(t_edge *edge);
void		move_edges_to_active(t_fill_data *fill);
void		sort_active_edges(t_edge **active_edges);
void		remove_finished_edges(t_fill_data *fill, t_edge **aet,
				int scanline_y);
void		increment_x_positions(t_edge *aet);
void		sort_intersections_with_windings(int *intersections,
				int *windings, int count);
int			fill_intersections_windings(t_fill_data *fill, t_edge *active_edges,
				int y);
void	render_subpixel_line(t_fill_data *fill, t_contour *contour, int *x);

/*
	Fill based on winding rule
*/
static void	fill_horizontal(t_fill_data *fill, int *windings,
		int *intersections, int intersection_count, t_contour *contour)
{
	int	winding_number;
	int	x[2];
	int	i;

	winding_number = 0;
	x[0] = -1;
	i = 0;
	while (i < intersection_count)
	{
		winding_number += windings[i];
		if ((x[0] == -1) && (winding_number != 0))
		{
			x[0] = intersections[i];
		}
		else if ((x[0] != -1) && (winding_number == 0))
		{
			x[1] = intersections[i];
			if (x[1] > x[0])
			{
				if (fill->env->subpixel)
					render_subpixel_line(fill, contour, x);
				else
				{
					int	xx[2];
					xx[0] = transform_x(contour, x[0]);
					xx[1] = transform_x(contour, x[1]);
					int yy = transform_y(contour, fill->y);
					ft_mlx_horizontal_line(&fill->env->mlx->img, xx, yy,
						fill->color);
				}
			}
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
		fill_horizontal(fill, fill->windings, fill->intersections,
			intersection_count, contour);
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
