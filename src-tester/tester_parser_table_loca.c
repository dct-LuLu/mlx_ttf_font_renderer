/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_table_loca.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 17:47:15 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/11 14:37:37 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "file_utils.h"
#include "libft.h"
#include "parser_font_ttf.h"

static void	debug_table_loca(t_loca_table loca, uint16_t num_glyphs)
{
	size_t	i;

	printf("LOCA Table:\n{\n\tformat: ");
	if (loca.format == 0)
		printf("short\n");
	else
		printf("long\n");
	printf("\tFirst %d offsets:\n\t{\n", DEBUG_NUM);
	i = 0;
	while ((i < DEBUG_NUM) && (i <= num_glyphs))
	{
		printf("\t\t[%zu] ", i);
		if (loca.format == 0)
			printf("%u\n", ((uint16_t *)(loca.offsets))[i]);
		else
			printf("%u\n", ((uint32_t *)(loca.offsets))[i]);
		i++;
	}
	printf("\t}\n}\n\n");
}

static void	endian_swap_table_loca(t_loca_table *loca, uint16_t num_glyphs)
{
	uint16_t	*offsets16;
	uint32_t	*offsets32;
	size_t		i;

	if (loca->format == 0)
	{
		offsets16 = (uint16_t *)loca->offsets;
		i = 0;
		while (i < num_glyphs)
		{
			offsets16[i] = be16toh(offsets16[i]);
			i++;
		}
	}
	else
	{
		offsets32 = (uint32_t *)loca->offsets;
		i = 0;
		while (i < num_glyphs)
		{
			offsets32[i] = be32toh(offsets32[i]);
			i++;
		}
	}
}

static int	continue_parse_table_loca(t_ttf_font *font,
		t_buffer *buf, const ssize_t loca_offset)
{
	const uint32_t	num_offsets = font->maxp->num_glyphs + 1;

	font->loca->format = font->head->index_to_loc_format;
	buf->pos = loca_offset;
	if (font->loca->format == 0)
		font->loca->offsets = ft_calloc(num_offsets, sizeof(uint16_t));
	else
		font->loca->offsets = ft_calloc(num_offsets, sizeof(uint32_t));
	if (!font->loca->offsets)
		return (error(errno, "loca offsets allocation"));
	if (font->loca->format == 0)
		read_bytes(buf, font->loca->offsets, num_offsets * sizeof(uint16_t));
	else
		read_bytes(buf, font->loca->offsets, num_offsets * sizeof(uint32_t));
	endian_swap_table_loca(font->loca, font->maxp->num_glyphs);
	return (0);
}

int	parse_table_loca(t_ttf_font *font, t_buffer *buf)
{
	const ssize_t	loca_offset = get_table_offset(font, LOCA_TAG);
	int				ret;

	if (loca_offset == -1)
		return (error(ERR_GET_OFFSET, ": loca"));
	font->loca = ft_calloc(sizeof(t_loca_table), 1);
	if (!font->loca)
		return (error(errno, "t_loca_table"));
	ret = continue_parse_table_loca(font, buf, loca_offset);
	if (ret)
		return (ret);
	if (DEBUG)
		debug_table_loca(*font->loca, font->maxp->num_glyphs);
	return (0);
}
