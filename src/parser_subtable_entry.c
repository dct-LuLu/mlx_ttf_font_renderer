/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_subtable_entry.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:31:36 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/15 17:02:15 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_font_ttf.h"
#include "endian_utils.h"
#include "libft.h"

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
			return (error(ERR_TABLE_ENTRIES, "bit mask: %d\ttag sum: %d",
					mask, sum));
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
static void	endian_swap_subtable_entry(t_subtable_entry *subtable_entry,
		const bool little_endian)
{
	if (little_endian)
	{
		subtable_entry->checksum = uswap32(subtable_entry->checksum);
		subtable_entry->offset = uswap32(subtable_entry->offset);
		subtable_entry->length = uswap32(subtable_entry->length);
	}
}

/*
	Function that reads throught all entries to get mandatory entries
	for font rendering
*/
int	read_subtable_entries(int fd, t_ttf_font *font, const bool little_endian)
{
	enum e_entry_tag	entry_tag;
	int					read_ret;
	int					entry_len;
	size_t				i;
	size_t				j;

	i = 0;
	j = 0;
	entry_len = sizeof(t_subtable_entry);
	while ((i < font->ot->num_tables) && (j < MANDATORY_TAG_NB))//peut etre besoin de lire plus
	{
		read_ret = read(fd, font->entries[j], entry_len);
		if (read_ret < 0)
			return (error(errno, NULL));
		if (read_ret < entry_len)
			return (error(ERR_CORRUPTED_FONT, "subtable_entry[%d] read size: %d",
					read_ret));
		endian_swap_subtable_entry(font->entries[j], little_endian);
		entry_tag = get_entry_tag_type(*(font->entries[j]));
		check_entries(font->entries[j], entry_tag, i);
		if (entry_tag)
			j++;
		i++;
	}
	return (check_entries(NULL, 0, 0));
}
