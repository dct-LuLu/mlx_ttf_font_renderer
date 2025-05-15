/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_table_maxp.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 15:53:48 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/15 15:58:21 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_font_ttf.h"
#include "libft.h"
#include "file_utils.h"
#include "endian_utils.h"

static void	debug_table_maxp(t_maxp_table maxp)
{
	printf("MAXP Table:\n{\n");
	printf("\tnum_glyphs: %u\n", maxp.num_glyphs);
	printf("}\n");
}

t_maxp_table	*parse_table_maxp(t_ttf_font *font, t_buffer *buf, const bool little_endian)
{
	const ssize_t	maxp_offset = get_table_offset(font, MAXP_TAG);
	t_maxp_table	*maxp;

	if (maxp_offset == -1)
		return (error(ERR_GET_OFFSET, ": maxp"), NULL);
	maxp = ft_calloc(sizeof(t_maxp_table), 1);
	if (maxp)
	{
		buf->pos = maxp_offset + 4;
		read_bytes(buf, &maxp->num_glyphs, 2);
		if (little_endian)
			maxp->num_glyphs = uswap16(maxp->num_glyphs);
		if (DEBUG)
			debug_table_maxp(*maxp);
	}
	return (maxp);
}
