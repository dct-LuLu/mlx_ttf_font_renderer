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
#include "endian_utils.h"
#include "file_utils.h"

int	read_subtables(const char *path, t_ttf_font *font, const bool little_endian)
{
	int	ret;
	int	fd;

	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (error(errno, ": %s", path));
	ret = read_subtable_offset(fd, font, little_endian);
	if (!ret)
		ret = read_subtable_entries(fd, font, little_endian);
	return (close(fd), ret);// check close
}

int	read_tables(const char *path, t_ttf_font *font, const bool little_endian)
{
	int	ret;

	ret = load_file(path, &font->buf);
	if (ret)
		return (ret);
	font->head = parse_table_head(font, font->buf, little_endian);
	if (!font->head)
		return (error(errno, ": t_head_table"));
	font->cmap = parse_table_cmap(font, font->buf, little_endian);
	if (!font->cmap)
		return (error(errno, ": t_cmap_table"));
	font->maxp = parse_table_maxp(font, font->buf, little_endian);
	if (!font->maxp)
		return (error(errno, ": t_maxp_table"));
	font->hhea = parse_table_hhea(font, font->buf, little_endian);
	if (!font->hhea)
		return (error(errno, ": t_hhea_table"));
	font->hmtx = parse_table_hmtx(font, font->buf, little_endian);
	if (!font->hmtx)
		return (error(errno, ": t_hmtx_table"));
	font->loca = parse_table_loca(font, font->buf, little_endian);
	if (!font->loca)
		return (error(errno, ": t_loca_table"));
	font->glyfs = parse_table_glyfs(font, font->buf, little_endian);
	if (!font->glyfs)
		return (error(errno, ": t_glyf_table"));
	return (0);
}

t_ttf_font	*read_ttf(const char *path)
{
	const bool	little_endian = is_little_endian();
	t_ttf_font	*font;
	int			ret;

	font = NULL;
	ret = init_ttf_struct(&font);
	if (ret)
		free_ttf(font);
	else
		ret = read_subtables(path, font, little_endian);
	if (ret)
		free_ttf(font);
	else
		ret = load_file(path, &font->buf);
	if (ret)
		free_ttf(font);
	else
		ret = read_tables(path, font, little_endian);
	if (ret)
		free_ttf(font);
	return (font);
}
