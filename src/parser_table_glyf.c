/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_table_glyf.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 17:41:23 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/15 18:37:06 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_font_ttf.h"
#include "libft.h"
#include "file_utils.h"
#include "endian_utils.h"

static void	debug_glyf_header(t_glyf_header header)
{
	printf("\tGlyph Header:\n\t{\n");
	printf("\t\tnumber_of_contours: %d\n", header.number_of_contours);
	printf("\t\tx_min: %d\n", header.x_min);
	printf("\t\ty_min: %d\n", header.y_min);
	printf("\t\tx_max: %d\n", header.x_max);
	printf("\t\ty_max: %d\n", header.y_max);
	printf("\t}\n");
}

static void	debug_table_glyf(t_glyf_table glyf)
{
	printf("Simple Glyph:\n{\n");
	debug_glyf_header(glyf.header);
	printf("\tpoint_count: %u\n", glyf.point_count);
	printf("\tend_pts[0]: %u\n", glyf.end_pts[0]);
	printf("}\n\n");
}

static void	endian_swap_glyf_header(t_glyf_header *header, const bool little_endian)
{
	if (little_endian)
	{
		header->number_of_contours = swap16(header->number_of_contours);
		header->x_min = swap16(header->x_min);
		header->y_min = swap16(header->y_min);
		header->x_max = swap16(header->x_max);
		header->y_max = swap16(header->y_max);
	}
}

static void	endian_swap_table_glyf(t_glyf_table *glyf, const bool little_endian)
{
	int	i;

	endian_swap_glyf_header(&glyf->header, little_endian);
	if (little_endian && glyf->end_pts)
	{
		i = 0;
		while (i < glyf->header.number_of_contours)
		{
			glyf->end_pts[i] = uswap16(glyf->end_pts[i]);
			i++;
		}
	}
}

static int	parse_glyf_header(t_glyf_table *glyf, t_buffer *buf, 
							const bool little_endian)
{
	uint16_t	instruction_length;
	uint16_t	i;
	uint16_t	last_point;
	
	// Read end points` of contours
	glyf->end_pts = ft_calloc(glyf->header.number_of_contours, sizeof(uint16_t));
	if (!glyf->end_pts)
		return (1);
	read_bytes(buf, glyf->end_pts, glyf->header.number_of_contours * 2);
	
	// Get the last point index (determines total points)
	last_point = glyf->end_pts[glyf->header.number_of_contours - 1];
	glyf->point_count = last_point + 1;
	
	// Skip instruction bytes
	read_bytes(buf, &instruction_length, 2);
	if (little_endian)
		instruction_length = uswap16(instruction_length);
	buf->pos += instruction_length;
	
	// Allocate and read flags
	glyf->flags = ft_calloc(glyf->point_count, sizeof(uint8_t));
	if (!glyf->flags)
		return (1);
	
	// Read flags with repeat handling
	i = 0;
	while (i < glyf->point_count)
	{
		uint8_t flag;
		uint8_t repeat_count = 1;
		
		read_bytes(buf, &flag, 1);
		glyf->flags[i] = flag;
		
		if (flag & 0x08) // REPEAT flag
		{
			read_bytes(buf, &repeat_count, 1);
			while (repeat_count > 0 && i + 1 < glyf->point_count)
			{
				glyf->flags[++i] = flag;
				repeat_count--;
			}
		}
		i++;
	}
	
	// Allocate coordinate arrays
	glyf->x_coordinates = ft_calloc(glyf->point_count, sizeof(int16_t));
	glyf->y_coordinates = ft_calloc(glyf->point_count, sizeof(int16_t));
	if (!glyf->x_coordinates || !glyf->y_coordinates)
		return (1);
	
	// Read X coordinates (variable format based on flags)
	int16_t x = 0;
	for (i = 0; i < glyf->point_count; i++)
	{
		if (glyf->flags[i] & 0x02) // X_SHORT
		{
			uint8_t delta;
			read_bytes(buf, &delta, 1);
			if (glyf->flags[i] & 0x10) // X_IS_POSITIVE
				x += delta;
			else
				x -= delta;
		}
		else if (!(glyf->flags[i] & 0x10)) // X_IS_SAME (bit not set)
		{
			int16_t delta;
			read_bytes(buf, &delta, 2);
			if (little_endian)
				delta = swap16(delta);
			x += delta;
		}
		// else: X_IS_SAME (bit set) means x += 0 (same as previous)
		
		glyf->x_coordinates[i] = x;
	}
	
	// Read Y coordinates (variable format based on flags)
	int16_t y = 0;
	for (i = 0; i < glyf->point_count; i++)
	{
		if (glyf->flags[i] & 0x04) // Y_SHORT
		{
			uint8_t delta;
			read_bytes(buf, &delta, 1);
			if (glyf->flags[i] & 0x20) // Y_IS_POSITIVE
				y += delta;
			else
				y -= delta;
		}
		else if (!(glyf->flags[i] & 0x20)) // Y_IS_SAME (bit not set)
		{
			int16_t delta;
			read_bytes(buf, &delta, 2);
			if (little_endian)
				delta = swap16(delta);
			y += delta;
		}
		// else: Y_IS_SAME (bit set) means y += 0 (same as previous)
		
		glyf->y_coordinates[i] = y;
	}
	
	endian_swap_table_glyf(glyf, little_endian);
	return (0);
}

t_glyf_table	*parse_table_glyf(t_ttf_font *font, t_buffer *buf, 
						uint16_t glyph_index, const bool little_endian)
{
	const ssize_t	glyf_offset = get_table_offset(font, GLYF_TAG);
	t_simple_glyph	*glyph;
	uint32_t		glyph_offset;
	uint32_t		next_glyph_offset;

	if (glyf_offset == -1)
		return (error(ERR_GET_OFFSET, ": glyf"), NULL);
	
	if (font->head->index_to_loc_format == 0)
	{
		uint16_t *offsets = (uint16_t *)font->loca->offsets;
		glyf_offset = offsets[glyf_index] * 2;
		next_glyf_offset = offsets[glyf_index + 1] * 2;
	}
	else
	{
		uint32_t *offsets = (uint32_t *)font->loca->offsets;
		glyf_offset = offsets[glyf_index];
		next_glyf_offset = offsets[glyf_index + 1];
	}
	
	// Check if this is an empty glyph (zero length)
	if (glyf_offset == next_glyf_offset)
		return (NULL);
	
	glyf = ft_calloc(sizeof(t_glyf_table), 1);
	if (!glyf)
		return (error(errno, "t_glyf_table"), NULL);
	
	// Position at glyph data
	buf->pos = glyf_offset + glyf_offset;
	
	// Read glyph header
	read_bytes(buf, &glyf->header.number_of_contours, 2);
	read_bytes(buf, &glyf->header.x_min, 2);
	read_bytes(buf, &glyf->header.y_min, 2);
	read_bytes(buf, &glyf->header.x_max, 2);
	read_bytes(buf, &glyf->header.y_max, 2);
		
	if (glyf->header.number_of_contours >= 0)
	{
		if (parse_glyf_header(glyf, buf, little_endian))
			return (NULL);
	}
	else
		return (NULL);
	if (DEBUG)
		debug_glyf_table(*glyf);
	return (glyf);
}

