
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    tester.mk                                            :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jaubry-- <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/21 12:15:21 by jaubry--          #+#    #+#              #
#    Updated: 2025/10/29 00:09:25 by jaubry--         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Directories
TESTER_DIR	= $(SRCDIR)/tester

# Source files
TESTER_SRCS	= main.c \
			  draw_routine.c \
			  key_hooks.c \
			  mouse_hooks.c \
			  setup.c \
			  mlx_fps.c \
			  save_file_utils.c
TESTER_SRCS	:= $(addprefix $(TESTER_DIR)/, $(TESTER_SRCS))

# VPATH
vpath %.c $(TESTER_DIR)
