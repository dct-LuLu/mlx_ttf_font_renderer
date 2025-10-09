/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_table_glyf_header.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 19:50:12 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/28 02:23:49 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "error_handler.h"
#include "file_utils.h"
#include "libft.h"
#include "parser_font_ttf.h"

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

static void	endian_swap_glyf_header(t_glyf_header *header)
{
	header->number_of_contours = be16toh(header->number_of_contours);
	header->x_min = be16toh(header->x_min);
	header->y_min = be16toh(header->y_min);
	header->x_max = be16toh(header->x_max);
	header->y_max = be16toh(header->y_max);
}

int	parse_glyf_header(t_glyf_table *glyf, t_buffer *buf)
{
	t_glyf_header	*glyf_header;

	glyf_header = ft_calloc(1, sizeof(t_glyf_header));
	if (!glyf_header)
		return (rerror(errno, "t_glyf_header"));
	read_bytes(buf, glyf_header, sizeof(t_glyf_header));
	endian_swap_glyf_header(glyf_header);
	glyf->header = glyf_header;
	return (0);
}
