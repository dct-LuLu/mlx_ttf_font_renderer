/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_font_ttf.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 11:53:08 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/28 05:46:59 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "file_utils.h"
#include "libft.h"
#include "parser_font_ttf.h"

int	read_subtables(t_ttf_font *font)
{
	int	ret;

	ret = read_subtable_offset(font);
	if (!ret)
		ret = read_subtable_entries(font);
	return (ret);
}

static int	read_tables(t_ttf_font *font)
{
	size_t				i;
	int					ret;
	const t_parse_func	parse_table[] = {parse_table_head, parse_table_cmap,
		parse_table_maxp, parse_table_hhea, parse_table_hmtx, parse_table_loca,
		parse_table_glyfs, NULL};

	ret = 0;
	i = 0;
	while (parse_table[i] && !ret)
	{
		ret = parse_table[i](font, font->buf);
		i++;
	}
	return (ret);
}

int	read_ttf(t_ttf_font *font, const char *path)
{
	int	ret;

	ret = load_file(path, &font->buf);
	if (!ret)
		ret = read_subtables(font);
	if (!ret)
		ret = read_tables(font);
	return (ret);
}
