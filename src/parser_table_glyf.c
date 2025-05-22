/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_table_glyf.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 17:41:23 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/22 02:27:59 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_font_ttf.h"
#include "libft.h"
#include "file_utils.h"



static void	debug_table_glyf(t_glyf_table glyf, size_t i)
{
	printf("\tGlyph %zu '%c':\n\t{\n", i, (char)i);
	debug_glyf_header(*(glyf.header));
	printf("\t\tpoint_count: %u\n", glyf.point_count);
	if (glyf.header->number_of_contours != -1)
		printf("\t\tend_pts[0]: %u\n", glyf.end_pts[0]);
	printf("\t}\n");
}

/*
static void	endian_swap_glyf(t_glyf_table *glyf)
{
	int16_t	i;

	if (glyf->end_pts)
	{
		i = 0;
		while (i < glyf->header->number_of_contours)
		{
			glyf->end_pts[i] = be16toh(glyf->end_pts[i]);
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
		next_glyf_offset = offsets16[glyf_index + 1] * 2;
	}
	else
	{
		offsets32 = (uint32_t *)font->loca->offsets;
		glyf_offset = offsets32[glyf_index];
		next_glyf_offset = offsets32[glyf_index + 1];
	}
	if (glyf_offset == next_glyf_offset)
		return (-1);
	return (glyf_offset);
}

static int	parse_table_glyf(t_glyf_table **glyf, t_ttf_font *font, t_buffer *buf, uint16_t glyf_index)
{
	const ssize_t	glyfs_table_offset = get_table_offset(font, GLYF_TAG);
	const ssize_t	glyf_offset = get_glyf_offset(font, glyf_index);
	static int	debug_nb = 0;

	if (glyfs_table_offset == -1)
		return (error(ERR_GET_OFFSET, ": glyf"));
	if (glyf_offset == -1)
		return (-1);
	*glyf = ft_calloc(sizeof(t_glyf_table), 1);
	if (!*glyf)
		return (error(errno, "t_glyf_table"));
	buf->pos = glyfs_table_offset + glyf_offset;
	if (parse_glyf_header(*glyf, buf))
		return (1);
	if ((*glyf)->header->number_of_contours >= 0)
	{
		if (parse_glyf(*glyf, buf))
			return (1);
	}
	else if ((*glyf)->header->number_of_contours == -1)
	{
		if (debug_nb < DEBUG_NUM)
		{
			debug_nb++;
			printf("composite glyph !\n");
		}
	}
	else
		return (1);
	return (0);
}

size_t get_glyph_index(t_ttf_font *font, size_t ch)
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
			idx = be16toh(*(uint16_t *)(font->buf->data +
					(cmap->pos_id_range_offset + (i * 2) +
					 cmap->id_range_offset[i] +
					 ((ch - cmap->start_code[i]) * 2))));
		return (idx + cmap->id_delta[i]);
    }
	return (0);
}


int	parse_table_glyfs(t_ttf_font *font, t_buffer *buf)
{
	int	i;
	int	ret;

	font->glyfs = ft_calloc(sizeof(t_glyf_table *), font->maxp->num_glyphs);
	if (!font->glyfs)
		return (error(errno, "t_glyf_table *"));
	i = 0;
	while (i < font->maxp->num_glyphs - 1)
	{
		ret = parse_table_glyf(&font->glyfs[i], font, buf, i);
		if ((ret == 0) && DEBUG && font->glyfs[i] && (i < DEBUG_NUM))
			debug_table_glyf(*(font->glyfs[i]), i);
		if (ret > 0)
			return (ret);
		i++;
	}
	return (0);
}
