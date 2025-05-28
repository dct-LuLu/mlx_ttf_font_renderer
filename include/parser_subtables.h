/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_subtables.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 10:07:38 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/28 04:15:16 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_SUBTABLES_H
# define PARSER_SUBTABLES_H

# include <stdint.h>

# define MANDATORY_TAG_NB 7
# define MANDATORY_TAG_BIT_MASK 0b11111110 // 254 in decimal
# define MANDATORY_TAG_SUM 28

# define SKIP_INSTRUCTION_BYTES 2

typedef enum e_entry_tag
{
	NON_MANDATORY_TAG,
	CMAP_TAG,
	GLYF_TAG,
	LOCA_TAG,
	HHEA_TAG,
	HMTX_TAG,
	HEAD_TAG,
	MAXP_TAG,
}					t_entry_tag;

# pragma pack(push, 1)

typedef struct s_offset_subtable
{
	uint32_t		sfnt_version;		// 0x00010000 for TTF
	uint16_t		num_tables;
	uint16_t		search_range;		// (2^floor(log2(num_tables))) * 16
	uint16_t		entry_selector;		// log2(search_range/16)
	uint16_t		range_shift;		// num_tables*16 search_range
}					t_offset_subtable;

typedef struct s_subtable_entry
{
	char			tag[4];				// e.g., 'cmap', 'glyf'
	uint32_t		checksum;			// Adler-32 validation
	uint32_t		offset;				// From file start
	uint32_t		length;				// Table size in bytes
}					t_subtable_entry;

# pragma pack(pop)

enum e_entry_tag	get_entry_tag_type(t_subtable_entry entry);

#endif // PARSER_SUBTABLES_H
