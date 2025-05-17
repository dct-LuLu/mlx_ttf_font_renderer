/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_table_glyf_header.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 19:50:12 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/16 22:17:42 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_font_ttf.h"
#include "file_utils.h"
#include "error_handler.h"
#include "endian_utils.h"
#include "libft.h"

void	debug_glyf_header(t_glyf_header header)
{
	printf("\t\tGlyph Header:\n\t\t{\n");
	printf("\t\t\tnumber_of_contours: %d\n", header.number_of_contours);
	printf("\t\t\tx_min: %d\n", header.x_min);
	printf("\t\t\ty_min: %d\n", header.y_min);
	printf("\t\t\tx_max: %d\n", header.x_max);
	printf("\t\t\ty_max: %d\n", header.y_max);
	printf("\t\t}\n");
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

t_glyf_header	*parse_glyf_header(t_buffer *buf, const bool little_endian)
{
	t_glyf_header	*glyf_header;

	glyf_header = ft_calloc(1, sizeof(t_glyf_header));
	if (!glyf_header)
		return (error(errno, "t_glyf_header"), NULL);
	read_bytes(buf, glyf_header, sizeof(t_glyf_header));
	//read_bytes(buf, &glyf->header.number_of_contours, 2);
	//read_bytes(buf, &glyf->header.x_min, 2);
	//read_bytes(buf, &glyf->header.y_min, 2);
	//read_bytes(buf, &glyf->header.x_max, 2);
	//read_bytes(buf, &glyf->header.y_max, 2);
	endian_swap_glyf_header(glyf_header, little_endian);
	return (glyf_header);
}
