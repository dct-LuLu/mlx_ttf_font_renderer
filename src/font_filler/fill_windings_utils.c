/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_windings_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 20:10:17 by jaubry--          #+#    #+#             */
/*   Updated: 2025/08/06 04:26:13 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rasterizer.h"

/*
	Sort intersections by x-coordinate while keeping windings aligned
*/
void	sort_intersections_with_windings(int *intersections, int *windings,
		int count)
{
	int		i;
	int		j;
	int		temp_x;
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
int	fill_intersections_windings(t_fill_data *fill, t_edge *active_edges, int y)
{
	t_edge	*current;
	int		i;
	int		intersection_count;

	intersection_count = count_valid_intersections_vertex(active_edges, y);
	if (intersection_count > MAX_INTERSECTIONS)
		intersection_count = MAX_INTERSECTIONS;
	i = 0;
	current = active_edges;
	while (current && i < intersection_count)
	{
		if (y < current->ymax)
		{
			fill->intersections[i] = current->x_current;
			fill->windings[i] = current->winding;
			i++;
		}
		current = current->next;
	}
	return (i);
}
