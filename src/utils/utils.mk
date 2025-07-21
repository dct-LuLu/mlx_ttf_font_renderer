# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    utils.mk                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jaubry-- <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/21 12:01:29 by jaubry--          #+#    #+#              #
#    Updated: 2025/07/21 12:53:56 by jaubry--         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Directories
UTILS_DIR		= $(SRCDIR)/utils

# Source files
UTILS_SRCS		= color_utils.c \
				  file_utils.c \
				  math_utils.c quad_bezier_utils.c \
				  scale_utils.c transform_utils.c \
				  time_utils.c \
				  vec2.c \
				  error_handler.c
UTILS_SRCS		:= $(addprefix $(UTILS_DIR)/, $(UTILS_SRCS))

SRCS			+= $(UTILS_SRCS)

# VPATH
vpath %.c $(UTILS_DIR)
