/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_font_ttf.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 10:18:00 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/15 18:17:49 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_FONT_TTF_H
# define PARSER_FONT_TTF_H
# include <stdlib.h>
# include <stdint.h>
# include <stdbool.h>
# include "error_handler.h"
# include "parser_subtables.h"
# include "parser_tables.h"

typedef struct s_ttf_font
{
	t_offset_subtable	*ot;
	t_subtable_entry	**entries;
	t_head_table		*head;
	t_maxp_table		*maxp;
	t_cmap_table		*cmap;
	t_hhea_table		*hhea;
	t_hmtx_table		*hmtx;
	t_loca_table		*loca;
	t_glyf_table		*glyf;
	t_buffer			*buf;
}						t_ttf_font;

int				init_ttf_struct(t_ttf_font **font);

int				read_ttf(const char *path);
void			free_ttf(t_ttf_font *font);

int				read_subtable_offset(int fd, t_ttf_font *font, const bool little_endian);
int				read_subtable_entries(int fd, t_ttf_font *font, const bool little_endian);

ssize_t			get_table_offset(t_ttf_font *font, enum e_entry_tag);

t_head_table	*parse_table_head(t_ttf_font *font, t_buffer *buf, const bool little_endian);
t_cmap_table	*parse_table_cmap(t_ttf_font *font, t_buffer *buf, const bool little_endian);
t_maxp_table	*parse_table_maxp(t_ttf_font *font, t_buffer *buf, const bool little_endian);
t_hhea_table	*parse_table_hhea(t_ttf_font *font, t_buffer *buf, const bool little_endian);
t_hmtx_table	*parse_table_hmtx(t_ttf_font *font, t_buffer *buf, const bool little_endian);
t_loca_table 	*parse_table_loca(t_ttf_font *font, t_buffer *buf, const bool little_endian);
t_glyf_table	*parse_table_glyf(t_ttf_font *font, t_buffer *buf, const bool little_endian);

#endif // PARSER_FONT_TTF_H
