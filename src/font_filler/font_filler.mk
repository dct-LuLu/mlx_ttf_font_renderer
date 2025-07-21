# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    font_filler.mk                                     :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jaubry-- <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/21 12:06:50 by jaubry--          #+#    #+#              #
#    Updated: 2025/07/21 12:24:39 by jaubry--         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Directories
FONT_FILLER_DIR		= $(SRCDIR)/font_filler

# Source files
FONT_FILLER_SRCS	= fill.c fill_utils.c \
					  fill_aet_utils.c fill_edge_pool_utils.c fill_edge_utils.c \
					  fill_contour_process.c fill_curve.c fill_curve_utils.c \
					  fill_scanline_process.c fill_subpixel_antialiasing.c \
					  fill_windings_utils.c
FONT_FILLER_SRCS	:= $(addprefix $(FONT_FILLER_DIR)/, $(FONT_FILLER_SRCS))

SRCS				+= $(FONT_FILLER_SRCS)

# VPATH
vpath %.c $(FONT_FILLER_DIR)
