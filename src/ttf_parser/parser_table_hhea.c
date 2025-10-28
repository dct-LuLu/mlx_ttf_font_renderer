/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_table_hhea.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 16:43:31 by jaubry--          #+#    #+#             */
/*   Updated: 2025/10/12 20:56:52 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "file_utils.h"
#include "libft.h"
#include "parser_font_ttf.h"

static void	debug_table_hhea(t_hhea_table hhea)
{
	printf("HHEA Table:\n{\n");
	printf("\tnum_lhmtx: %u\n", hhea.num_lhmtx);
	printf("}\n\n");
}

int	parse_table_hhea(t_ttf_font *font, t_buffer *buf)
{
	const ssize_t	hhea_offset = get_table_offset(font, HHEA_TAG);
	t_hhea_table	*hhea;

	if (hhea_offset == -1)
	{
		register_complex_err_msg(FRDR_E_MSG_OFFSET, "HHEA table");
		return (error(pack_err(FRDR_ID, FRDR_E_OFFSET), FL, LN, FC));
	}
	hhea = ft_calloc(sizeof(t_hhea_table), 1);
	if (!hhea)
		return (-1);
	buf->pos = hhea_offset + 34;
	read_bytes(buf, &hhea->num_lhmtx, 2);
	hhea->num_lhmtx = be16toh(hhea->num_lhmtx);
	if (DEBUG)
		debug_table_hhea(*hhea);
	font->hhea = hhea;
	return (0);
}
