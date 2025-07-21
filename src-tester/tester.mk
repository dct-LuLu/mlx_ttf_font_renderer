# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    tester.mk                                          :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jaubry-- <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/21 14:38:16 by jaubry--          #+#    #+#              #
#    Updated: 2025/07/21 14:52:08 by jaubry--         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS		= main.c parser_font_ttf.c \
			  parser_subtable_offset.c parser_subtable_entry.c \
			  parser_table_head.c \
			  parser_table_cmap.c parser_table_cmap_utils.c \
			  parser_table_maxp.c \
			  parser_table_hhea.c \
			  parser_table_hmtx.c \
			  parser_table_loca.c \
			  parser_table_glyf.c parser_table_glyf_header.c \
			  parser_table_glyf_simple.c parser_table_glyf_composite.c parser_table_glyf_utils.c \
			  render_bounding_box.c render_glyph.c render_contour.c render_utils.c \
			  render_curves.c render_curves_utils.c \
			  fill_aet_utils.c fill_contour_process.c fill_scanline_process.c fill.c fill_utils.c \
			  fill_windings_utils.c fill_curve_utils.c fill_edge_utils.c fill_edge_pool_utils.c fill_curve.c \
			  text_alignment.c text_rendering.c \
			  render_subpixel_antialiasing.c \
			  parser_tag_utils.c \
			  init_utils.c \
			  free_utils1.c free_utils2.c free_utils3.c \
			  file_utils.c \
			  time_utils.c \
			  save_utils.c \
			  color_utils.c \
			  math_utils.c \
			  transform_utils.c \
			  scale_utils.c \
			  error_handler.c \
			  img_utils.c mlx_draw.c mlx_draw_utils1.c mlx_draw_utils2.c mlx_font_renderer.c mlx_draw_curves.c \
			  mlx_hooks.c mlx_fps.c vec2.c
SRCS		:= $(addprefix $(SRCDIR)/tester_,$(SRCS))

vpath %.c $(SRCDIR)
