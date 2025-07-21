# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    mlx_layer.mk                                       :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jaubry-- <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/21 12:21:12 by jaubry--          #+#    #+#              #
#    Updated: 2025/07/21 12:23:03 by jaubry--         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Directories
MLX_LAYER_DIR	= $(SRCDIR)/mlx_layer

# Source files
MLX_LAYER_SRCS	= mlx_draw_curves.c \
				  mlx_draw_utils1.c mlx_draw_utils2.c \
				  mlx_img_utils.c
MLX_LAYER_SRCS	:= $(addprefix $(MLX_LAYER_DIR)/, $(MLX_LAYER_SRCS))

SRCS			+= $(MLX_LAYER_SRCS)

# VPATH
vpath %.c $(MLX_LAYER_DIR)
