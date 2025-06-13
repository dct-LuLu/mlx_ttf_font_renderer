/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_windings_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 20:10:17 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/13 22:14:16 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

/*
	Sort intersections by x-coordinate while keeping windings aligned
*/
void	sort_intersections_with_windings(float *intersections, int *windings,
		int count)
{
	int		i;
	int		j;
	float	temp_x;
	int		temp_w;

	i = 1;
	while (i < count)
	{
		temp_x = intersections[i];
		temp_w = windings[i];
		j = i - 1;
		while ((j >= 0) && (intersections[j] > temp_x))
		{
			intersections[j + 1] = intersections[j];
			windings[j + 1] = windings[j];
			j--;
		}
		intersections[j + 1] = temp_x;
		windings[j + 1] = temp_w;
		i++;
	}
}

/*
	Function that count the valid intersection vertex
	(checks if the scanline doesnt hit the edge's maximum vertex)
*/
static int	count_valid_intersections_vertex(t_edge *active_edges, int y)
{
	int		intersection_count;
	t_edge	*current;

	intersection_count = 0;
	current = active_edges;
	while (current)
	{
		if (y < current->ymax)
			intersection_count++;
		current = current->next;
	}
	return (intersection_count);
}

/*
	Function to collect valid intersections and windings.
*/
int	fill_intersections_windings(t_edge *active_edges, float **intersections,
		int **windings, int y)
{
	t_edge		*current;
	int			i;
	const int	intersection_count = count_valid_intersections_vertex(
			active_edges, y);

	*intersections = malloc(sizeof(float) * intersection_count);
	*windings = malloc(sizeof(int) * intersection_count);
	i = 0;
	current = active_edges;
	while (current)
	{
		if (y < current->ymax)
		{
			(*intersections)[i] = current->x_current;
			(*windings)[i] = current->winding;
			i++;
		}
		current = current->next;
	}
	return (intersection_count);
}

/*
	Determine if contour is clockwise (outer) or counter-clockwise (inner)
	Returns 1 for clockwise (outer), -1 for counter-clockwise (inner)
	And computes the signed area using hte shoelace formula
*/
int	get_contour_winding_direction(t_contour *contour, t_curve_params params)
{
	float	signed_area;
	int		curr_idx;
	int		next_idx;
	t_vec2	p1;
	t_vec2	p2;

	signed_area = 0.0f;
	curr_idx = params.contour_start;
	while (curr_idx <= params.contour_end)
	{
		next_idx = curr_idx + 1;
		if (curr_idx == params.contour_end)
			next_idx = params.contour_start;
		p1 = new_screen_pt(contour, contour->glyf->x_coordinates[curr_idx],
				contour->glyf->y_coordinates[curr_idx]);
		p2 = new_screen_pt(contour, contour->glyf->x_coordinates[next_idx],
				contour->glyf->y_coordinates[next_idx]);
		signed_area += (p2.x - p1.x) * (p2.y + p1.y);
		curr_idx++;
	}
	return (-1 + (2 * (signed_area > 0)));
}
