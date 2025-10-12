# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    srcs.mk                                            :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jaubry-- <jaubry--@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/12 18:46:14 by jaubry--          #+#    #+#              #
#    Updated: 2025/10/12 19:01:07 by jaubry--         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

MKS			= font_drawer/font_drawer.mk font_filler/font_filler.mk \
			  ttf_parser/ttf_parser.mk text_renderer/text_renderer.mk \
			  utils/utils.mk
include $(addprefix $(SRCDIR)/, $(MKS))

SRCS		+= register_frdr_errors.c

# VPATH
vpath %.c $(SRCDIR)
