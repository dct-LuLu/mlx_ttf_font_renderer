/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 21:52:54 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/13 22:07:41 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

/*
	Updates x positions for all edges in active edge list for next scanline
*/
void	increment_x_positions(t_edge *aet)
{
	t_edge	*current;

	current = aet;
	while (current)
	{
		current->x_current += current->inv_slope;
		current = current->next;
	}
}

static void	cleanup_edges(t_edge *edge)
{
	t_edge	*next;

	while (edge)
	{
		next = edge->next;
		free(edge);
		edge = next;
	}
}

/*
	Frees all memory allocated for fill data
*/
void	cleanup_fill_data(t_fill_data *fill)
{
	int		i;

	if (fill->edge_table)
	{
		i = 0;
		while (i < fill->height)
		{
			cleanup_edges(fill->edge_table[i]);
			i++;
		}
		free(fill->edge_table);
	}
	cleanup_edges(fill->active_edges);
}
