/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_edge_pool_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:09:52 by jaubry--          #+#    #+#             */
/*   Updated: 2025/08/06 09:06:43 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rasterizer.h"

void	return_edge_to_pool(t_edge_pool *pool, t_edge *edge)
{
	int	idx;

	idx = edge - pool->edges;
	if (idx >= 0 && idx < MAX_ACTIVE_EDGES
		&& pool->free_count < MAX_ACTIVE_EDGES)
	{
		pool->free_indices[pool->free_count] = idx;
		pool->free_count++;
	}
}

void	remove_edge(t_fill_data *fill, t_edge **aet, t_edge **prev,
		t_edge **curr)
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
	return_edge_to_pool(&fill->edge_pool, temp);
}

static t_edge	*get_edge_from_pool(t_edge_pool *pool)
{
	int	idx;

	if (pool->free_count <= 0)
		return (NULL);
	pool->free_count--;
	idx = pool->free_indices[pool->free_count];
	return (&pool->edges[idx]);
}

t_edge	*init_edge(t_fill_data *fill, t_vec2i p1, t_vec2i p2)
{
	t_edge	*edge;

	edge = get_edge_from_pool(&fill->edge_pool);
	if (!edge)
		return (NULL);
	if (p1.y < p2.y)
	{
		edge->x_current = p1.x;
		edge->ymax = p2.y;
		edge->winding = -1;
	}
	else
	{
		edge->x_current = p2.x;
		edge->ymax = p1.y;
		edge->winding = 1;
	}
	return (edge);
}
