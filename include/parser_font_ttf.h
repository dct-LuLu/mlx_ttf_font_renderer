/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_font_ttf.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 10:18:00 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/26 13:08:51 by jaubry--         ###   ########.fr       */
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

# ifndef DEBUG
#  define DEBUG 0
# endif //DEBUG

# ifndef DEBUG_NUM
#  define DEBUG_NUM 5
# endif //DEBUG_NUM

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
	t_glyf_table		**glyfs;
	t_buffer			*buf;
}						t_ttf_font;

int			init_ttf_struct(t_ttf_font **font);

int			read_ttf(t_ttf_font *font, const char *path);

int			read_subtable_offset(t_ttf_font *font);
int			read_subtable_entries(t_ttf_font *font);

ssize_t		get_table_offset(t_ttf_font *font, enum e_entry_tag);

int			parse_table_head(t_ttf_font *font, t_buffer *buf);
int			parse_table_cmap(t_ttf_font *font, t_buffer *buf);
int			parse_table_maxp(t_ttf_font *font, t_buffer *buf);
int			parse_table_hhea(t_ttf_font *font, t_buffer *buf);
int			parse_table_hmtx(t_ttf_font *font, t_buffer *buf);
int			parse_table_loca(t_ttf_font *font, t_buffer *buf);

int			parse_table_glyfs(t_ttf_font *font, t_buffer *buf);
int			parse_glyf_header(t_glyf_table *glyf, t_buffer *buf);
int			parse_simple_glyf(t_glyf_table *glyf, t_buffer *buf);
int			parse_composite_glyf(t_glyf_table *glyf, t_buffer *buf);
void		debug_glyf_header(t_glyf_header header);
void		debug_glyf_component(t_glyf_component comp);

size_t		get_glyph_index(t_ttf_font *font, size_t ch);

#endif // PARSER_FONT_TTF_H
