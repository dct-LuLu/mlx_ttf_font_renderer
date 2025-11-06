# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    test.mk                                            :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jaubry-- <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/21 12:15:21 by jaubry--          #+#    #+#              #
#    Updated: 2025/10/29 00:09:25 by jaubry--         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Directories
TEST_DIR	= $(SRCDIR)/test

# Source files
TEST_SRCS	= main.c \
			  mlx_font_renderer.c draw_routine.c \
			  mlx_fps.c \
			  render_text_test.c
TEST_SRCS	:= $(addprefix $(TEST_DIR)/, $(TEST_SRCS))

# VPATH
vpath %.c $(TEST_DIR)
