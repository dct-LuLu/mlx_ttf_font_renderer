/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 21:52:54 by jaubry--          #+#    #+#             */
/*   Updated: 2025/08/06 04:26:13 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rasterizer.h"

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
