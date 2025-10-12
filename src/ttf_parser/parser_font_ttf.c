/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_font_ttf.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 11:53:08 by jaubry--          #+#    #+#             */
/*   Updated: 2025/10/12 20:36:25 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "file_utils.h"
#include "libft.h"
#include "parser_font_ttf.h"

static int	read_subtables(t_ttf_font *font)
{
	if (read_subtable_offset(font) != 0)
		return (error(pack_err(FRDR_ID, FRDR_E_MTABLE), FL, LN, FC));
	if (read_subtable_entries(font) != 0)
		return (error(pack_err(FRDR_ID, FRDR_E_ENTRIES), FL, LN, FC));
	return (0);
}

static int	read_tables(t_ttf_font *font)
{
	size_t				i;
	const t_parse_func	parse_table[] = {parse_table_head, parse_table_cmap,
		parse_table_maxp, parse_table_hhea, parse_table_hmtx, parse_table_loca, parse_table_glyfs, NULL};
	const char			*strtag_table[] = {"HEAD", "CMAP", "MAXP", "HHEA", "HMTX", "LOCA", "GLYF", NULL};

	i = 0;
	while (parse_table[i])
	{
		if (parse_table[i](font, font->buf) != 0)
		{
			register_complex_err_msg(FRDR_E_MSG_RTABLE, strtag_table[i]);
			return (error(pack_err(FRDR_ID, FRDR_E_RTABLE), FL, LN, FC));
		}
		i++;
	}
	return (0);
}

static int	read_ttf(t_ttf_font *font, const char *path)
{
	if (load_file(path, &font->buf) != 0)
		return (error(pack_err(FRDR_ID, FRDR_E_FBUFF), FL, LN, FC));
	if (read_subtables(font) != 0)
		return (error(pack_err(FRDR_ID, FRDR_E_SUBTABLE), FL, LN, FC));
	if (read_tables(font) != 0)
		return (error(pack_err(FRDR_ID, FRDR_E_TABLE), FL, LN, FC));
	return (0);
}

int	init_ttf(const char *path, t_ttf_font **ptr)
{
	if (!path || (ft_strrncmp(path, ".ttf", 4) != 0))
		return (error(pack_err(FRDR_ID, FRDR_E_EXT), FL, LN, FC));
	if (init_ttf_struct(ptr) != 0)
		return (error(pack_err(FRDR_ID, FRDR_E_TTFSTRUCT), FL, LN, FC));
	if (read_ttf(*ptr, path) != 0)
	{
		register_complex_err_msg(FRDR_E_MSG_FETCH, ".ttf data");
		return (error(pack_err(FRDR_ID, FRDR_E_FETCH), FL, LN, FC));
	}
	return (0);
}
