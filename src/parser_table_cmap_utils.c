/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_table_cmap_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 11:39:31 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/11 11:41:46 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "file_utils.h"
#include "libft.h"
#include "parser_font_ttf.h"

void	debug_table_cmap(t_cmap_table cmap)
{
	size_t	i;

	printf("CMAP Table:\n{\n");
	printf("\tformat: %u\n", cmap.format);
	printf("\tseg_count: %u\tseg_count_x2: %u\n", cmap.seg_count,
		cmap.seg_count_x2);
	i = 0;
	printf("\tSEG:\tend_code, \tstart_code, \tid_delta,\
		\t\tid_range_offset:\n\t{\n");
	while (i < cmap.seg_count && i < DEBUG_NUM)
	{
		printf("\t\t%u\t\t%u\t\t%d\t\t\t%u\n", cmap.end_code[i],
			cmap.start_code[i], cmap.id_delta[i], cmap.id_range_offset[i]);
		i++;
	}
	printf("\t}\n");
	i = 0;
	printf("\n\tglyph_id_count: %u\n", cmap.glyph_id_count);
	printf("\n\tglyph_id_array:\n\t{\n");
	while (i < cmap.glyph_id_count && i < DEBUG_NUM)
	{
		printf("\t%u", cmap.glyph_id_array[i]);
		i++;
	}
	printf("\t}\n");
	printf("}\n\n");
}

void	endian_swap_table_cmap(t_cmap_table *cmap)
{
	size_t	i;

	i = 0;
	while (i < cmap->seg_count)
	{
		cmap->end_code[i] = be16toh(cmap->end_code[i]);
		cmap->start_code[i] = be16toh(cmap->start_code[i]);
		cmap->id_delta[i] = be16toh(cmap->id_delta[i]);
		cmap->id_range_offset[i] = be16toh(cmap->id_range_offset[i]);
		i++;
	}
	i = 0;
	while (i < cmap->glyph_id_count)
	{
		cmap->glyph_id_array[i] = be16toh(cmap->glyph_id_array[i]);
		i++;
	}
}
