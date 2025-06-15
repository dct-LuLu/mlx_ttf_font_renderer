/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_edge_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 21:44:29 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/15 20:18:54 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

void	return_edge_to_pool(t_edge_pool *pool, t_edge *edge);
void	remove_edge(t_fill_data *fill, t_edge **aet, t_edge **prev,
			t_edge **curr);
t_edge	*init_edge(t_fill_data *fill, t_vec2 p1, t_vec2 p2);

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

void	remove_finished_edges(t_fill_data *fill, t_edge **aet, int scanline_y)
{
	t_edge	*prev;
	t_edge	*curr;

	prev = NULL;
	curr = *aet;
	while (curr)
	{
		if (curr->ymax <= scanline_y)
			remove_edge(fill, aet, &prev, &curr);
		else
		{
			prev = curr;
			curr = curr->next;
		}
	}
}

void	add_edge(t_fill_data *fill, t_vec2 p1, t_vec2 p2)
{
	t_edge	*edge;
	int		bucket_y;

	if (p1.y == p2.y)
		return ;
	edge = init_edge(fill, p1, p2);
	if (!edge)
		return ;
	edge->inv_slope = (float)(p2.x - p1.x) / (float)(p2.y - p1.y);
	if (p1.y < p2.y)
		bucket_y = p1.y - fill->y_min;
	else
		bucket_y = p2.y - fill->y_min;
	if ((bucket_y >= 0) && (bucket_y < fill->height)
		&& (bucket_y < MAX_GLYPH_HEIGHT))
	{
		edge->next = fill->edge_table[bucket_y];
		fill->edge_table[bucket_y] = edge;
	}
	else
		return_edge_to_pool(&fill->edge_pool, edge);
}
