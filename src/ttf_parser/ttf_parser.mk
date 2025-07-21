# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    ttf_parser.mk                                      :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jaubry-- <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/21 11:48:19 by jaubry--          #+#    #+#              #
#    Updated: 2025/07/21 12:55:15 by jaubry--         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Directories
TTF_PARSER_DIR	= $(SRCDIR)/ttf_parser

# Source files
TTF_PARSER_SRCS	= parser_font_ttf.c \
				  parser_subtable_entry.c parser_subtable_offset.c \
				  parser_table_cmap.c parser_table_cmap_utils.c \
				  parser_table_head.c \
				  parser_table_hhea.c \
				  parser_table_hmtx.c \
				  parser_table_loca.c \
				  parser_table_maxp.c \
				  parser_tag_utils.c \
				  parser_table_glyf.c parser_table_glyf_composite.c \
				  parser_table_glyf_header.c parser_table_glyf_simple.c \
				  parser_table_glyf_utils.c \
				  free_utils1.c free_utils2.c free_utils3.c \
				  init_utils.c
TTF_PARSER_SRCS	:= $(addprefix $(TTF_PARSER_DIR)/, $(TTF_PARSER_SRCS))

SRCS			+= $(TTF_PARSER_SRCS)

# VPATH
vpath %.c $(TTF_PARSER_DIR)
