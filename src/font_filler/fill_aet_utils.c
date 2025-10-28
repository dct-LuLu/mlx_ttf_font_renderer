/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_aet_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 21:52:42 by jaubry--          #+#    #+#             */
/*   Updated: 2025/08/06 04:26:13 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rasterizer.h"

/*
	Move edges from edge table to active edge table
*/
void	move_edges_to_active(t_fill_data *fill)
{
	t_edge	*edge;
	t_edge	*next;
	int		bucket_idx;

	bucket_idx = fill->y - fill->y_min;
	if ((bucket_idx < 0) || (bucket_idx >= fill->height)
		|| (bucket_idx >= MAX_GLYPH_HEIGHT))
		return ;
	edge = fill->edge_table[bucket_idx];
	while (edge)
	{
		next = edge->next;
		edge->next = fill->active_edges;
		fill->active_edges = edge;
		edge = next;
	}
	fill->edge_table[bucket_idx] = NULL;
}

static int	swap_edges(t_edge **current, t_edge **next)
{
	t_edge	*tmp;

	tmp = *current;
	*current = *next;
	*next = tmp;
	return (1);
}

static int	sort_single_edge(t_edge ***active_edges, t_edge **prev,
		t_edge **current, t_edge **next)
{
	int	swapped;

	swapped = 0;
	*prev = NULL;
	*current = **active_edges;
	while (*current && (*current)->next)
	{
		*next = (*current)->next;
		if ((*current)->x_current > (*next)->x_current)
		{
			(*current)->next = (*next)->next;
			(*next)->next = *current;
			if (*prev)
				(*prev)->next = *next;
			else
				**active_edges = *next;
			swapped = swap_edges(current, next);
		}
		*prev = *current;
		*current = (*current)->next;
	}
	return (swapped);
}

/*
	Sort active edges by x coordinate (linked list sorting)
*/
void	sort_active_edges(t_edge **active_edges)
{
	t_edge	*current;
	t_edge	*next;
	t_edge	*prev;
	int		swapped;

	if (!active_edges || !*active_edges || !(*active_edges)->next)
		return ;
	swapped = 1;
	while (swapped)
	{
		swapped = sort_single_edge(&active_edges, &prev, &current, &next);
	}
}
