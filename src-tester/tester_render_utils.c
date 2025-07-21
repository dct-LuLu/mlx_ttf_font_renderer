/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:48:09 by jaubry--          #+#    #+#             */
/*   Updated: 2025/07/16 20:13:10 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"
#include "parser_tables.h"
#include "vec2.h"

//inline
t_vec2	get_glyf_coords(t_glyf_table *glyf, int idx)
{
	return (new_vec2(glyf->x_coordinates[idx], glyf->y_coordinates[idx]));
}
