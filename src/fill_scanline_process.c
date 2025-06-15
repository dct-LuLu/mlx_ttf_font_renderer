/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_scanline_process.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 21:52:45 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/15 20:18:44 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

int			count_edges(t_edge *edge);
void		move_edges_to_active(t_fill_data *fill);
void		sort_active_edges(t_edge **active_edges);
void		remove_finished_edges(t_fill_data *fill, t_edge **aet,
				int scanline_y);
void		increment_x_positions(t_edge *aet);
void		sort_intersections_with_windings(float *intersections,
				int *windings, int count);
int			fill_intersections_windings(t_fill_data *fill, t_edge *active_edges,
				int y);

/*
	Fill based on winding rule
*/
static void	fill_horizontal(t_fill_data *fill, int *windings,
		float *intersections, int intersection_count)
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
			x[0] = (int)(intersections[i] + 0.5f);
		else if ((x[0] != -1) && (winding_number == 0))
		{
			x[1] = (int)(intersections[i] + 0.5f);
			if (x[1] > x[0])
				ft_mlx_horizontal_line(&fill->env->mlx->img, x, fill->y,
					fill->color);
			x[0] = -1;
		}
		i++;
	}
}

/*
	Enhanced scanline filling with proper vertex intersection rules
*/
static void	fill_scanline_horizontal(t_fill_data *fill, t_edge *active_edges)
{
	const int	intersection_count = fill_intersections_windings(fill,
			active_edges, fill->y);

	if (intersection_count != 0)
	{
		sort_intersections_with_windings(fill->intersections, fill->windings,
			intersection_count);
		fill_horizontal(fill, fill->windings, fill->intersections,
			intersection_count);
	}
}

void	fill_scanline_process(t_fill_data *fill)
{
	int	total_lines_drawn;
	int	edges_after;

	total_lines_drawn = 0;
	fill->y = fill->y_min;
	while (fill->y <= fill->y_max)
	{
		move_edges_to_active(fill);
		edges_after = count_edges(fill->active_edges);
		if (edges_after > 0)
		{
			sort_active_edges(&fill->active_edges);
			fill_scanline_horizontal(fill, fill->active_edges);
			total_lines_drawn++;
		}
		remove_finished_edges(fill, &fill->active_edges, fill->y);
		increment_x_positions(fill->active_edges);
		fill->y++;
	}
}
