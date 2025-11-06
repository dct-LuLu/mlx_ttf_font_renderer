# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    text_renderer.mk                                   :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jaubry-- <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/21 12:30:17 by jaubry--          #+#    #+#              #
#    Updated: 2025/07/21 12:51:49 by jaubry--         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Directories
TEXT_RENDERER_DIR	= $(SRCDIR)/text_renderer

# Source files
TEXT_RENDERER_SRCS	= render_glyph.c
TEXT_RENDERER_SRCS	:= $(addprefix $(TEXT_RENDERER_DIR)/, $(TEXT_RENDERER_SRCS))

SRCS				+= $(TEXT_RENDERER_SRCS)

# VPATH
vpath %.c $(TEXT_RENDERER_DIR)
