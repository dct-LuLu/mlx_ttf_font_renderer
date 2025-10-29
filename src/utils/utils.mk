# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    utils.mk                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/21 12:01:29 by jaubry--          #+#    #+#              #
#    Updated: 2025/10/12 21:17:05 by jaubry--         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Directories
UTILS_DIR		= $(SRCDIR)/utils

# Source files
UTILS_SRCS		= file_utils.c \
				  scale_utils.c transform_utils.c
UTILS_SRCS		:= $(addprefix $(UTILS_DIR)/, $(UTILS_SRCS))

SRCS			+= $(UTILS_SRCS)

# VPATH
vpath %.c $(UTILS_DIR)
