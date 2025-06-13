/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_edge_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 21:44:29 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/13 22:16:35 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

int	count_edges(t_edge *edge)
{
	int	count;

	count = 0;
	while (edge)
	{
		count++;
		edge = edge->next;
	}
	return (count);
}

static void	free_linked_edge(t_edge **aet, t_edge **prev, t_edge **curr)
{
	t_edge	*temp;

	if (*prev)
	{
		(*prev)->next = (*curr)->next;
		temp = *curr;
		*curr = (*curr)->next;
	}
	else
	{
		temp = *curr;
		*aet = (*curr)->next;
		*curr = *aet;
	}
	free(temp);
}

/*
	Removes edges from active edge list that have reached their maximum y value
*/
void	remove_finished_edges(t_edge **aet, int scanline_y)
{
	t_edge	*prev;
	t_edge	*curr;

	prev = NULL;
	curr = *aet;
	while (curr)
	{
		if (curr->ymax <= scanline_y)
			free_linked_edge(aet, &prev, &curr);
		else
		{
			prev = curr;
			curr = curr->next;
		}
	}
}

static t_edge	*init_edge(t_vec2 p1, t_vec2 p2, int contour_direction)
{
	t_edge	*edge;
	int		base_winding;

	edge = malloc(sizeof(t_edge));
	if (p1.y < p2.y)
	{
		edge->x_current = p1.x;
		edge->ymax = p2.y;
		base_winding = -1;
	}
	else
	{
		edge->x_current = p2.x;
		edge->ymax = p1.y;
		base_winding = 1;
	}
	edge->winding = base_winding * contour_direction;
	return (edge);
}

void	add_edge(t_fill_data *fill, t_vec2 p1, t_vec2 p2, int contour_direction)
{
	t_edge	*edge;
	int		bucket_y;

	if (p1.y == p2.y)
		return ;
	edge = init_edge(p1, p2, contour_direction);
	edge->inv_slope = (float)(p2.x - p1.x) / (float)(p2.y - p1.y);
	if (p1.y < p2.y)
		bucket_y = p1.y - fill->y_min;
	else
		bucket_y = p2.y - fill->y_min;
	if ((bucket_y >= 0) && (bucket_y < fill->height))
	{
		edge->next = fill->edge_table[bucket_y];
		fill->edge_table[bucket_y] = edge;
	}
	else
		free(edge);
}
