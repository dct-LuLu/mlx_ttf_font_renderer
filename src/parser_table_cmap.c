/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_table_cmap.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 10:40:32 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/21 20:27:46 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parser_font_ttf.h"
#include "libft.h"
#include "file_utils.h"


static void	debug_table_cmap(t_cmap_table cmap)
{
	size_t	i;

	printf("CMAP Table:\n{\n");
	printf("\tformat: %u\n", cmap.format);
	printf("\tseg_count: %u\tseg_count_x2: %u\n", cmap.seg_count, cmap.seg_count_x2);
	i = 0;
	printf("\tSEG:\tend_code, \tstart_code, \tid_delta, \t\tid_range_offset:\n\t{\n");
	while (i < cmap.seg_count && i < DEBUG_NUM)
	{
		printf("\t\t%u\t\t%u\t\t%d\t\t\t%u\n", cmap.end_code[i], cmap.start_code[i], cmap.id_delta[i], cmap.id_range_offset[i]);
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

static void	endian_swap_table_cmap(t_cmap_table *cmap)
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

static int	parse_table_cmap_seg(t_cmap_table *cmap, t_buffer *buf)
{
	cmap->end_code = ft_calloc(cmap->seg_count, sizeof(uint16_t));
	if (!cmap->end_code)
		return (1);
	read_bytes(buf, cmap->end_code, cmap->seg_count_x2);
	buf->pos += 2;
	cmap->start_code = ft_calloc(cmap->seg_count, sizeof(uint16_t));
	if (!cmap->start_code)
		return (1);
	read_bytes(buf, cmap->start_code, cmap->seg_count_x2);
	cmap->id_delta = ft_calloc(cmap->seg_count, sizeof(int16_t));
	if (!cmap->id_delta)
		return (1);
	read_bytes(buf, cmap->id_delta, cmap->seg_count_x2);
	cmap->id_range_offset = ft_calloc(cmap->seg_count, sizeof(uint16_t));
	if (!cmap->id_range_offset)
		return (1);
	//cmap->id_range_offset_ptr = (void *)(buf->data + buf->pos);
	cmap->pos_id_range_offset = buf->pos;
	read_bytes(buf, cmap->id_range_offset, cmap->seg_count_x2);
	return (0);
}

static int	parse_table_cmap_id_array(t_cmap_table *cmap, t_buffer *buf, uint16_t length)
{
	size_t	header_and_arrays_size;
	size_t	remaining_bytes;

	header_and_arrays_size = 16 + (cmap->seg_count * 8);
	remaining_bytes = length - header_and_arrays_size;
	if (remaining_bytes > 0)
	{
		cmap->glyph_id_count = remaining_bytes / 2;
		cmap->glyph_id_array = ft_calloc(remaining_bytes, 1);
		if (!cmap->glyph_id_array)
			return (1);
		read_bytes(buf, cmap->glyph_id_array, remaining_bytes);
	}
	return (0);
}

int	pre_parse_table_cmap(t_buffer *buf, const ssize_t cmap_offset)
{
	uint16_t	num_tables;
	uint16_t	platform_id;
	uint16_t	encoding_id;
	uint32_t	subtable_offset;
	uint32_t	format4_offset;
	size_t		i;

	format4_offset = 0;
	buf->pos = cmap_offset + 2;
	read_bytes(buf, &num_tables, 2);
	i = 0;
	while (i < num_tables)
	{
		read_bytes(buf, &platform_id, 2);
		platform_id = be16toh(platform_id);
		read_bytes(buf, &encoding_id, 2);
		encoding_id = be16toh(encoding_id);
		read_bytes(buf, &subtable_offset, 4);
		subtable_offset = be32toh(subtable_offset);
		if (((platform_id == 3) && (encoding_id == 1))
			|| ((platform_id == 0) && (encoding_id == 3)))
		{
			format4_offset = cmap_offset + subtable_offset;
			break ;
		}
		i++;
	}
	if (format4_offset == 0)
		return (1);
	buf->pos = format4_offset;
	return (0);
}

int	parse_table_cmap(t_ttf_font *font, t_buffer *buf)
{
	const ssize_t	cmap_offset = get_table_offset(font, CMAP_TAG);
	t_cmap_table	*cmap;
	uint16_t		length;

	if (cmap_offset == -1)
		return (error(ERR_GET_OFFSET, ": cmap"));
	cmap = ft_calloc(sizeof(t_cmap_table), 1);
	if (!cmap)
		return (error(errno, ": t_cmap_table"));
	if (pre_parse_table_cmap(buf, cmap_offset))
		return (1);
	read_bytes(buf, &cmap->format, 2);
	cmap->format = be16toh(cmap->format);
	if (cmap->format != 4)
		return (1);
	read_bytes(buf, &length, 2);
	length = be16toh(length);
	buf->pos += 2;
	read_bytes(buf, &cmap->seg_count_x2, 2);
	cmap->seg_count_x2 = be16toh(cmap->seg_count_x2);
	cmap->seg_count = cmap->seg_count_x2 >> 1;
	buf->pos += 6;
	if (parse_table_cmap_seg(cmap, buf))
		return (1); // free table cmap
	buf->pos += 2;
	if (parse_table_cmap_id_array(cmap, buf, length))
		return (1); // free table cmap
	endian_swap_table_cmap(cmap);
	if (DEBUG)
		debug_table_cmap(*cmap);
	font->cmap = cmap;
	return (0);
}
