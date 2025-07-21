/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_subtable_entry.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:31:36 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/28 02:23:35 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "parser_font_ttf.h"

static void	debug_subtable_entry(t_subtable_entry subtable_entry, size_t i)
{
	char	tag[5];

	ft_memcpy(tag, subtable_entry.tag, 4);
	tag[4] = '\0';
	printf("Table entry %zu:\n{\n", i + 1);
	ft_printf("\ttag: '%s'\n", tag);
	ft_printf("\tchecksum: %u\n", subtable_entry.checksum);
	ft_printf("\toffset: %u\n", subtable_entry.offset);
	ft_printf("\tlength: %u\n", subtable_entry.length);
	ft_printf("}\n\n");
}

/*
	Function that compute the bit masking value and the sum of the tags ot
	check if all mandatory entries are present, will check if it's the case
	upon calling it with NULL and 0
*/
static int	check_entries(t_subtable_entry *subtable_entry,
		enum e_entry_tag entry_tag, size_t i)
{
	static int	mask = 0;
	static int	sum = 0;

	if ((subtable_entry == NULL) && (entry_tag == 0))
	{
		if ((mask != MANDATORY_TAG_BIT_MASK) || (sum != MANDATORY_TAG_SUM))
			return (error(ERR_TABLE_ENTRIES, "bit mask: %d\ttag sum: %d", mask,
					sum));
		return (0);
	}
	if (entry_tag)
	{
		if (DEBUG)
			debug_subtable_entry(*subtable_entry, i);
		mask |= (1 << entry_tag);
		sum += entry_tag;
	}
	return (0);
}

/*
	Function that check endianness and will swap if necessary
*/
static void	endian_swap_subtable_entry(t_subtable_entry *subtable_entry)
{
	subtable_entry->checksum = be32toh(subtable_entry->checksum);
	subtable_entry->offset = be32toh(subtable_entry->offset);
	subtable_entry->length = be32toh(subtable_entry->length);
}

/*
	Function that reads throught all entries to get mandatory entries
	for font rendering
*/
int	read_subtable_entries(t_ttf_font *font)
{
	enum e_entry_tag	entry_tag;
	size_t				entry_nb;
	size_t				i;

	i = 0;
	entry_nb = 0;
	while ((i < font->ot->num_tables) && (entry_nb < MANDATORY_TAG_NB))
	{
		read_bytes(font->buf, font->entries[entry_nb],
			sizeof(t_subtable_entry));
		endian_swap_subtable_entry(font->entries[entry_nb]);
		entry_tag = get_entry_tag_type(*(font->entries[entry_nb]));
		check_entries(font->entries[entry_nb], entry_tag, i);
		if (entry_tag)
			entry_nb++;
		i++;
	}
	return (check_entries(NULL, 0, 0));
}
