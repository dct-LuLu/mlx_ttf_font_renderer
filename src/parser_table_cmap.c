/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_table_cmap.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 10:40:32 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/11 11:41:28 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "file_utils.h"
#include "libft.h"
#include "parser_font_ttf.h"

void	debug_table_cmap(t_cmap_table cmap);
void	endian_swap_table_cmap(t_cmap_table *cmap);

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
	cmap->pos_id_range_offset = buf->pos;
	read_bytes(buf, cmap->id_range_offset, cmap->seg_count_x2);
	return (0);
}

static int	parse_table_cmap_id_array(t_cmap_table *cmap, t_buffer *buf,
		uint16_t length)
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
	uint16_t	id[2];
	uint32_t	offsets[2];

	offsets[0] = 0;
	buf->pos = cmap_offset + 2;
	read_bytes(buf, &num_tables, 2);
	while (num_tables > 0)
	{
		read_bytes(buf, id, 4);
		id[0] = be16toh(id[0]);
		id[1] = be16toh(id[1]);
		read_bytes(buf, &offsets[1], 4);
		offsets[1] = be32toh(offsets[1]);
		if (((id[0] == 3) && (id[1] == 1)) || ((id[0] == 0) && (id[1] == 3)))
		{
			offsets[0] = cmap_offset + offsets[1];
			break ;
		}
		num_tables--;
	}
	if (offsets[0] == 0)
		return (1);
	buf->pos = offsets[0];
	return (0);
}

static int	continue_parse_table_cmap(t_cmap_table *cmap,
		t_buffer *buf, uint16_t length)
{
	buf->pos += 2;
	read_bytes(buf, &cmap->seg_count_x2, 2);
	cmap->seg_count_x2 = be16toh(cmap->seg_count_x2);
	cmap->seg_count = cmap->seg_count_x2 >> 1;
	buf->pos += 6;
	if (parse_table_cmap_seg(cmap, buf))
		return (1);
	buf->pos += 2;
	if (parse_table_cmap_id_array(cmap, buf, length))
		return (1);
	endian_swap_table_cmap(cmap);
	if (DEBUG)
		debug_table_cmap(*cmap);
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
	if (continue_parse_table_cmap(cmap, buf, length) != 0)
		return (1);
	font->cmap = cmap;
	return (0);
}
