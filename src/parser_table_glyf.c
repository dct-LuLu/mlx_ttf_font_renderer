/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_table_glyf.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 17:41:23 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/28 02:24:33 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "file_utils.h"
#include "libft.h"
#include "parser_font_ttf.h"

static void	debug_table_glyf(t_glyf_table glyf, size_t i)
{
	printf("\tGlyph %zu '%c':\n\t{\n", i, (char)i);
	debug_glyf_header(*(glyf.header));
	printf("\t\tpoint_count: %u\n", glyf.point_count);
	if (glyf.header->number_of_contours == -1)
		debug_glyf_component(*(glyf.component));
	// printf("\t\tend_pts[0]: %u\n", glyf.end_pts[0]);
	printf("\t}\n");
}

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

static int	parse_table_glyf(t_glyf_table **glyf, t_ttf_font *font,
		t_buffer *buf, uint16_t glyf_index)
{
	const ssize_t	glyfs_table_offset = get_table_offset(font, GLYF_TAG);
	const ssize_t	glyf_offset = get_glyf_offset(font, glyf_index);

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
	if ((*glyf)->header->number_of_contours > 0)
		return (parse_simple_glyf(*glyf, buf));
	else if ((*glyf)->header->number_of_contours == -1)
		return (parse_composite_glyf(*glyf, buf));
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
