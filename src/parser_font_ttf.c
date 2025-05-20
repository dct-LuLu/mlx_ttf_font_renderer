/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_font_ttf.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 11:53:08 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/17 00:06:50 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "parser_font_ttf.h"
#include "libft.h"

#include "file_utils.h"

int	read_subtables(const char *path, t_ttf_font *font)
{
	int	ret;
	int	fd;

	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (error(errno, ": %s", path));
	ret = read_subtable_offset(fd, font);
	if (!ret)
		ret = read_subtable_entries(fd, font);
	return (close(fd), ret);// check close
}

static int	read_tables(const char *path, t_ttf_font *font)
{
	size_t	i;
	int		ret;
	int		(* const parse_table[])(t_ttf_font *, t_buffer *) = {
		parse_table_head,
		parse_table_cmap,
		parse_table_maxp,
		parse_table_hhea,
		parse_table_hmtx,
		parse_table_loca,
		parse_table_glyfs,
		NULL
	};

	ret = load_file(path, &font->buf);
	i = 0;
	while (parse_table[i] && !ret)
	{
		ret = parse_table[i](font, font->buf);
		i++;
	}
	//if (ret)
	//	free_ttf(font);
	return (ret);
	//return (error(errno, ": t_glyf_table"));
}

t_ttf_font	*read_ttf(const char *path)
{
	t_ttf_font	*font;
	int			ret;

	font = NULL;
	ret = init_ttf_struct(&font);
	if (ret)
		free_ttf(font);
	else
		ret = read_subtables(path, font);
	if (ret)
		free_ttf(font);
	else
		ret = load_file(path, &font->buf);
	if (ret)
		free_ttf(font);
	else
		ret = read_tables(path, font);
	if (ret)
		free_ttf(font);
	return (font);
}
