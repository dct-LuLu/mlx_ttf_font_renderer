/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_table_hhea.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 16:43:31 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/15 16:56:43 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_font_ttf.h"
#include "libft.h"
#include "file_utils.h"
#include "endian_utils.h"

static void debug_table_hhea(t_hhea_table hhea)
{
    printf("HHEA Table:\n{\n");
    printf("\tnum_lhmtx: %u\n", hhea.num_lhmtx);
    printf("}\n\n");
}

static void endian_swap_table_hhea(t_hhea_table *hhea, const bool little_endian)
{
    if (little_endian)
    {
        hhea->num_lhmtx = uswap16(hhea->num_lhmtx);
    }
}

t_hhea_table *parse_table_hhea(t_ttf_font *font, t_buffer *buf, const bool little_endian)
{
	const ssize_t hhea_offset = get_table_offset(font, HHEA_TAG);
	t_hhea_table *hhea;

	if (hhea_offset == -1)
		return (error(ERR_GET_OFFSET, ": hhea"), NULL);
    hhea = ft_calloc(sizeof(t_hhea_table), 1);
    if (!hhea)
        return (error(errno, "t_hhea_table"), NULL);
    buf->pos = hhea_offset + 34;
    read_bytes(buf, &hhea->num_lhmtx, 2);
    endian_swap_table_hhea(hhea, little_endian);
    if (DEBUG)
        debug_table_hhea(*hhea);
    return (hhea);
}
