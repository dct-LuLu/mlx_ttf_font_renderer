/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_table_maxp.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 15:53:48 by jaubry--          #+#    #+#             */
/*   Updated: 2025/10/12 20:56:12 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "file_utils.h"
#include "libft.h"
#include "parser_font_ttf.h"

static void	debug_table_maxp(t_maxp_table maxp)
{
	printf("MAXP Table:\n{\n");
	printf("\tnum_glyphs: %u\n", maxp.num_glyphs);
	printf("}\n");
}

int	parse_table_maxp(t_ttf_font *font, t_buffer *buf)
{
	const ssize_t	maxp_offset = get_table_offset(font, MAXP_TAG);
	t_maxp_table	*maxp;

	if (maxp_offset == -1)
	{
		register_complex_err_msg(FRDR_E_MSG_OFFSET, "MAXP table");
		return (error(pack_err(FRDR_ID, FRDR_E_OFFSET), FL, LN, FC));
	}
	maxp = ft_calloc(sizeof(t_maxp_table), 1);
	if (!maxp)
		return (-1);
	buf->pos = maxp_offset + 4;
	read_bytes(buf, &maxp->num_glyphs, 2);
	maxp->num_glyphs = be16toh(maxp->num_glyphs);
	if (DEBUG)
		debug_table_maxp(*maxp);
	font->maxp = maxp;
	return (0);
}
