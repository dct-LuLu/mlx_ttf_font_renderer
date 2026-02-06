# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    utils.mk                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/21 12:01:29 by jaubry--          #+#    #+#              #
#    Updated: 2026/02/06 22:59:42 by jaubry--         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Directories
UTILS_DIR		= $(SRCDIR)/utils

# Source files
UTILS_SRCS		= load_file_utils.c read_file_utils.c \
				  scale_utils.c transform_utils.c \
				  measure_width_utils.c measure_height_utils.c \
				  get_contour.c
UTILS_SRCS		:= $(addprefix $(UTILS_DIR)/, $(UTILS_SRCS))

SRCS			+= $(UTILS_SRCS)

# VPATH
vpath %.c $(UTILS_DIR)
