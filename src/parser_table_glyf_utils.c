/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_table_glyf_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:14:20 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/28 02:24:07 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parser_font_ttf.h"

size_t	get_glyph_index(t_ttf_font *font, size_t ch)
{
	t_cmap_table	*cmap;
	size_t			i;
	size_t			idx;

	cmap = font->cmap;
	i = 0;
	while (i < cmap->seg_count)
	{
		if (!((ch <= cmap->end_code[i]) && (ch >= cmap->start_code[i])))
		{
			i++;
			continue ;
		}
		if (cmap->id_range_offset[i] == 0)
			idx = ch;
		else
			idx = be16toh(*(uint16_t *)(font->buf->data
						+ (cmap->pos_id_range_offset + (i * 2)
							+ cmap->id_range_offset[i] + ((ch
									- cmap->start_code[i]) * 2))));
		return (idx + cmap->id_delta[i]);
	}
	return (0);
}
