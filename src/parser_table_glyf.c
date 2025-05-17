/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_table_glyf.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 17:41:23 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/17 00:50:05 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_font_ttf.h"
#include "libft.h"
#include "file_utils.h"
#include "endian_utils.h"


static void	debug_table_glyf(t_glyf_table glyf, size_t i)
{
	printf("\tGlyph %zu '%c':\n\t{\n", i, (char)i);
	debug_glyf_header(*(glyf.header));
	printf("\t\tpoint_count: %u\n", glyf.point_count);
	printf("\t\tend_pts[0]: %u\n", glyf.end_pts[0]);
	printf("\t}\n");
}

/*
static void	endian_swap_glyf(t_glyf_table *glyf, const bool little_endian)
{
	int16_t	i;

	if (little_endian && glyf->end_pts)
	{
		i = 0;
		while (i < glyf->header->number_of_contours)
		{
			glyf->end_pts[i] = uswap16(glyf->end_pts[i]);
			i++;
		}
	}
}
*/

static ssize_t	get_glyf_offset(t_ttf_font *font, uint16_t glyf_index)
{
	uint16_t	*offsets16;
	uint32_t	*offsets32;
	size_t		glyf_offset;
	size_t		next_glyf_offset;

	if (font->head->index_to_loc_format == 0)
	{
		offsets16 = (uint16_t *)font->loca->offsets;
		glyf_offset = offsets16[glyf_index] * 2;
		next_glyf_offset = offsets16[glyf_index + 1] * 2;// multiply by 2 ? vraiment ?
	}
	else
	{
		offsets32 = (uint32_t *)font->loca->offsets;
		glyf_offset = offsets32[glyf_index];
		next_glyf_offset = offsets32[glyf_index + 1];
	}
	// Check if this is an empty glyph (zero length)
	if (glyf_offset == next_glyf_offset)
		return (-1);
	return (glyf_offset);
}


static t_glyf_table	*parse_table_glyf(t_ttf_font *font, t_buffer *buf, uint16_t glyf_index, const bool little_endian)
{
	const ssize_t	glyfs_table_offset = get_table_offset(font, GLYF_TAG);
	const ssize_t	glyf_offset = get_glyf_offset(font, glyf_index);
	t_glyf_table			*glyf;

	if (glyfs_table_offset == -1)
		return (error(ERR_GET_OFFSET, ": glyf"), NULL);
	if (glyf_offset == -1)
		return (NULL);
	glyf = ft_calloc(sizeof(t_glyf_table), 1);
	if (!glyf)
		return (error(errno, "t_glyf_table"), NULL);
	buf->pos = glyfs_table_offset + glyf_offset;
	glyf->header = parse_glyf_header(buf, little_endian);
	if (!glyf->header)
		return (NULL);
	if (glyf->header->number_of_contours >= 0)
	{
		if (parse_glyf(glyf, buf, little_endian))
			return (NULL);
	}
	else
		return (NULL);
	return (glyf);
}

size_t	get_glyph_index(t_ttf_font *font, size_t ch)
{
    t_cmap_table	*cmap = font->cmap;
    uint16_t		idx;
    uint16_t		glyph_id;
    size_t	i;
    
    i = 0;
    // Search through segments to find the one containing char_code
	while (i < cmap->seg_count)
    {
        if (ch <= cmap->end_code[i])
        {
            // Check if character is in this segment
            if (ch >= cmap->start_code[i])
            {
                // Direct mapping: glyphId = (char_code + idDelta) % 65536
                if (cmap->id_range_offset[i] == 0)
                    return ((ch + cmap->id_delta[i]) & 0xFFFF);
                else
                {
                    // Indirect mapping with glyphIdArray
                    idx = (cmap->id_range_offset[i] / 2) + (ch - cmap->start_code[i]);
                    // Get the glyph ID from the array
                    glyph_id = *(cmap->glyph_id_array + idx);
                    // If not 0, add idDelta
                    if (glyph_id != 0)
                        return ((glyph_id + cmap->id_delta[i]) & 0xFFFF);
                    // If 0, return 0 (missing glyph)
                    return (0);
                }
            }
            break ;
        }
        i++;
    }
    // Character not found, return 0 (missing glyph)
    return (0);
}


t_glyf_table	**parse_table_glyfs(t_ttf_font *font, t_buffer *buf, const bool little_endian)
{
	t_glyf_table	**glyfs;
	size_t			i;
	size_t			glyf_i;	

	glyfs = ft_calloc(sizeof(t_glyf_table *), font->maxp->num_glyphs);
	if (!glyfs)
		return (error(errno, "t_glyf_table *"), NULL);
	i = 0;
	while ((i < 256) && (i < font->maxp->num_glyphs))
	{
		glyf_i = get_glyph_index(font, i);
		glyfs[i] = parse_table_glyf(font, buf, glyf_i, little_endian);
		if (DEBUG && glyfs[i] && (i < DEBUG_NUM))
			debug_table_glyf(*(glyfs[i]), i);
		i++;
	}
	return (glyfs);
}
