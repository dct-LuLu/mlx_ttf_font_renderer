/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_table_loca.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 17:47:15 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/15 18:22:31 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_font_ttf.h"
#include "libft.h"
#include "file_utils.h"
#include "endian_utils.h"

static void debug_table_loca(t_loca_table loca, uint16_t num_glyphs)
{
	size_t	i;

    printf("LOCA Table:\n{\n\tformat: ");
    if (loca.format == 0)
    	printf("short\n");
    else
    	printf("long\n");
    printf("\tFirst 5 offsets:\n\t{\n");
    i = 0;
    while ((i < 5) && (i <= num_glyphs))
    {
        printf("\t\t[%zu] ", i);
    	if (loca.format == 0)
    		printf("%u\n", *(uint16_t *)(loca.offsets + i));
    	else
    		printf("%u\n", *(uint32_t *)(loca.offsets + i));
    	i++;
    }
    printf("\t}\n}\n\n");
}

static void endian_swap_table_loca(t_loca_table *loca, uint16_t num_glyphs, const bool little_endian)
{
	uint16_t	*offsets16;
	uint32_t	*offsets32;
	size_t		i;

    if (little_endian)
    {
        if (loca->format == 0) // 16-bit offsets
        {
            offsets16 = (uint16_t *)loca->offsets;
            i = 0;
            while (i < num_glyphs)
            {
                offsets16[i] = uswap16(offsets16[i]);
                i++;
            }
        }
        else // 32-bit offsets
        {
            offsets32 = (uint32_t *)loca->offsets;
            i = 0;
            while (i < num_glyphs)
            {
                offsets32[i] = swap32(offsets32[i]);
                i++;
            }
        }
    }
}

t_loca_table *parse_table_loca(t_ttf_font *font, t_buffer *buf, const bool little_endian)
{
    const ssize_t loca_offset = get_table_offset(font, LOCA_TAG);
    t_loca_table *loca;
    uint16_t num_glyphs;

    if (loca_offset == -1)
        return (error(ERR_GET_OFFSET, ": loca"), NULL);
    loca = ft_calloc(sizeof(t_loca_table), 1);
    if (!loca)
        return (error(errno, "t_loca_table"), NULL);
    loca->format = font->head->index_to_loc_format;
    num_glyphs = font->maxp->num_glyphs;
    const uint32_t num_offsets = num_glyphs + 1;
    
    buf->pos = loca_offset;
    if (loca->format == 0)
        loca->offsets = ft_calloc(num_offsets, sizeof(uint16_t));
	else
        loca->offsets = ft_calloc(num_offsets, sizeof(uint32_t));
    if (!loca->offsets)
        return (error(errno, "loca offsets allocation"), NULL);
    if (loca->format == 0)
        read_bytes(buf, loca->offsets, num_offsets * sizeof(uint16_t));
    else
        read_bytes(buf, loca->offsets, num_offsets * sizeof(uint32_t));
    
    endian_swap_table_loca(loca, num_glyphs, little_endian);
    if (DEBUG)
        debug_table_loca(*loca, num_glyphs);
    return (loca);
}

