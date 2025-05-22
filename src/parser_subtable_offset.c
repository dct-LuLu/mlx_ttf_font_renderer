/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_subtable_offset.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:29:45 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/22 00:07:39 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_font_ttf.h"
#include "libft.h"
#include "file_utils.h"


static void	debug_offset_subtable(t_offset_subtable offset_subtable)
{
	//static char const *test[5] = {""}

	ft_printf("Offset Subtable:\n{\n");
	printf("\tsfnt_version: 0x%08X\n", offset_subtable.sfnt_version);
	ft_printf("\tnum_tables: %u\n", offset_subtable.num_tables);
	ft_printf("\tsearch_range: %u\n", offset_subtable.search_range);
	ft_printf("\tentry_selector: %u\n", offset_subtable.entry_selector);
	ft_printf("\trange_shift: %u\n", offset_subtable.range_shift);
	ft_printf("}\n\n");
}

/*
	Function that check endianness and will swap if necessary
*/
static void	endian_swap_offset_subtable(t_offset_subtable *ot)
{
	ot->sfnt_version = be32toh(ot->sfnt_version);
	ot->num_tables = be16toh(ot->num_tables);
	ot->search_range = be16toh(ot->search_range);
	ot->entry_selector = be16toh(ot->entry_selector);
	ot->range_shift = be16toh(ot->range_shift);
	// verify values snft, and possible tables
}

/*
	Function that reads the offset_subtable of the font
*/
int	read_subtable_offset(t_ttf_font *font)
{
	read_bytes(font->buf, font->ot, sizeof(t_offset_subtable));
	endian_swap_offset_subtable(font->ot);
	if (DEBUG)
		debug_offset_subtable(*(font->ot));
	return (0);
}
