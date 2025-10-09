/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_table_head.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 14:36:42 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/28 02:24:36 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "file_utils.h"
#include "libft.h"
#include "parser_font_ttf.h"

static void	debug_table_head(t_head_table head)
{
	printf("HEAD Table:\n{\n");
	printf("\tunits_per_em: %u\n", head.units_per_em);
	printf("\tx_min: %d\n", head.x_min);
	printf("\ty_min: %d\n", head.y_min);
	printf("\tx_max: %d\n", head.x_max);
	printf("\ty_max: %d\n", head.y_max);
	printf("\tindex_to_loc_format: %u\n", head.index_to_loc_format);
	printf("}\n\n");
}

static void	endian_swap_table_head(t_head_table *head)
{
	head->units_per_em = be16toh(head->units_per_em);
	head->x_min = be16toh(head->x_min);
	head->y_min = be16toh(head->y_min);
	head->x_max = be16toh(head->x_max);
	head->y_max = be16toh(head->y_max);
	head->index_to_loc_format = be16toh(head->index_to_loc_format);
}

int	parse_table_head(t_ttf_font *font, t_buffer *buf)
{
	const ssize_t	head_offset = get_table_offset(font, HEAD_TAG);
	t_head_table	*head;

	if (head_offset == -1)
		return (rerror(ERR_GET_OFFSET, ": head"));
	head = ft_calloc(sizeof(t_head_table), 1);
	if (!head)
		return (rerror(errno, ":t_head_table"));
	buf->pos = head_offset + 18;
	read_bytes(buf, &head->units_per_em, 2);
	buf->pos = head_offset + 36;
	read_bytes(buf, &head->x_min, 2);
	read_bytes(buf, &head->y_min, 2);
	read_bytes(buf, &head->x_max, 2);
	read_bytes(buf, &head->y_max, 2);
	buf->pos = head_offset + 50;
	read_bytes(buf, &head->index_to_loc_format, 2);
	endian_swap_table_head(head);
	if (DEBUG)
		debug_table_head(*head);
	font->head = head;
	return (0);
}
