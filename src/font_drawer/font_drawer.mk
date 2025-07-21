# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    font_drawer.mk                                     :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jaubry-- <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/21 12:33:24 by jaubry--          #+#    #+#              #
#    Updated: 2025/07/21 12:56:53 by jaubry--         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Directories
FONT_DRAWER_DIR		= $(SRCDIR)/font_drawer

# Source files
FONT_DRAWER_SRCS	= draw_outline.c draw_utils.c \
					  draw_curves.c draw_curves_utils.c \
					  debug_draw_bounding_box.c
FONT_DRAWER_SRCS	:= $(addprefix $(FONT_DRAWER_DIR)/, $(FONT_DRAWER_SRCS))

SRCS				+= $(FONT_DRAWER_SRCS)

# VPATH
vpath %.c $(FONT_DRAWER_DIR)
