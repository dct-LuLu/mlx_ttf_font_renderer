/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_subtable_offset.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:29:45 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/15 15:54:18 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parser_font_ttf.h"
#include "libft.h"
#include "endian_utils.h"

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
static void	endian_swap_offset_subtable(t_offset_subtable *ot, const bool little_endian)
{
	if (little_endian)
	{
		ot->sfnt_version = uswap32(ot->sfnt_version);
		ot->num_tables = uswap16(ot->num_tables);
		ot->search_range = uswap16(ot->search_range);
		ot->entry_selector = uswap16(ot->entry_selector);
		ot->range_shift = uswap16(ot->range_shift);
		// verify values snft, and possible tables
	}
}

/*
	Function that reads the offset_subtable of the font
*/
int	read_subtable_offset(int fd, t_ttf_font *font, const bool little_endian)
{
	int	ot_len;
	int	read_ret;

	ot_len = sizeof(t_offset_subtable);
	read_ret = read(fd, font->ot, ot_len);
	if (read_ret < 0)
		return (error(errno, NULL));
	if (read_ret < ot_len)
		return (error(ERR_CORRUPTED_FONT,
				"offset_subtable read size: %d", read_ret));
	endian_swap_offset_subtable(font->ot, little_endian);
	if (DEBUG)
		debug_offset_subtable(*(font->ot));
	return (0);
}
