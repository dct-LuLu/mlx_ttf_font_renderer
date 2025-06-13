/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_scanline_process.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 21:52:45 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/13 22:11:43 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

int			count_edges(t_edge *edge);
void		remove_finished_edges(t_edge **aet, int scanline_y);
void		increment_x_positions(t_edge *aet);
void		sort_intersections_with_windings(float *intersections,
				int *windings, int count);
int			fill_intersections_windings(t_edge *active_edges,
				float **intersections, int **windings, int y);

/*
	Fill based on winding rule
*/
static void	fill_horizontal(t_contour *contour, int *windings,
		float *intersections, int intersection_count, int y)
{
	int	winding_number;
	int	x_start;
	int	x_end;
	int	i;

	winding_number = 0;
	x_start = -1;
	i = 0;
	while (i < intersection_count)
	{
		winding_number += windings[i];
		if (x_start == -1 && winding_number != 0)
			x_start = (int)(intersections[i] + 0.5f);
		else if ((x_start != -1) && (winding_number == 0))
		{
			x_end = (int)(intersections[i] + 0.5f);
			if (x_end > x_start)
				ft_mlx_horizontal_line(&contour->env->mlx->img,
					x_start, x_end, y, contour->color);
			x_start = -1;
		}
		i++;
	}
}

/*
	Enhanced scanline filling with proper vertex intersection rules
*/
static void	fill_scanline_horizontal(t_contour *contour,
		t_edge *active_edges, int y)
{
	float		*intersections;
	int			*windings;
	const int	intersection_count = fill_intersections_windings(active_edges,
			&intersections, &windings, y);

	if (intersection_count != 0)
	{
		sort_intersections_with_windings(intersections, windings,
			intersection_count);
		fill_horizontal(contour, windings, intersections, intersection_count, y);
	}
	free(intersections);
	free(windings);
}

/*
	Main scanline fill function
*/
void	fill_scanline_process(t_contour *contour, t_fill_data *fill)
{
	int	current_y;
	int	total_lines_drawn;
	int	edges_after;

	total_lines_drawn = 0;
	current_y = fill->y_min;
	while (current_y <= fill->y_max)
	{
		move_edges_to_active(fill, current_y);
		edges_after = count_edges(fill->active_edges);
		if (edges_after > 0)
		{
			sort_active_edges(&fill->active_edges);
			fill_scanline_horizontal(contour, fill->active_edges,
				current_y);
			total_lines_drawn++;
		}
		remove_finished_edges(&fill->active_edges, current_y);
		increment_x_positions(fill->active_edges);
		current_y++;
	}
}
