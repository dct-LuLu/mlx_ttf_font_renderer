/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:48:09 by jaubry--          #+#    #+#             */
/*   Updated: 2025/08/06 09:16:25 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rasterizer.h"
#include "parser_tables.h"
#include "vectors.h"

//inline
t_vec2i	get_glyf_coords(t_glyf_table *glyf, int idx)
{
	return (vec2i(glyf->x_coordinates[idx], glyf->y_coordinates[idx]));
}
