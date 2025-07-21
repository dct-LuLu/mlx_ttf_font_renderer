/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 17:32:54 by jaubry--          #+#    #+#             */
/*   Updated: 2025/07/16 20:13:39 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "math_utils.h"

/*
	Euclidean distance, achieving approximately 3.5% error rate.
*/
int	fast_distance(t_vec2 a, t_vec2 b)
{
	const int	dx = abs(b.x - a.x);
	const int	dy = abs(b.y - a.y);
	int			min_val;

	if (dx < dy)
		min_val = dx;
	else
		min_val = dy;
	return (dx + dy - (min_val >> 1) - (min_val >> 2) + (min_val >> 4));
}

//inline
float	clamp(float value, float min_val, float max_val)
{
	return (fmaxf(min_val, fminf(value, max_val)));
}
