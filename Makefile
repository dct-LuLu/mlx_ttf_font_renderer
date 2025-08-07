# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/11 10:16:04 by jaubry--          #+#    #+#              #
#    Updated: 2025/08/07 05:31:19 by jaubry--         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SHELL := /bin/bash

# Print utils
include ../../colors.mk

# Variables
DEBUG		= $(if $(filter debug,$(MAKECMDGOALS)),1,0)
WIDTH		= 500
HEIGHT		= 500

# Directories
CDIR		= font_renderer
SRCDIR		= src
INCDIR		= include
OBJDIR		= .obj
DEPDIR		= .dep
LIBFTDIR	= ../libft
MLXDIR		= ../minilibx-linux

# Output
NAME		= libfont_renderer.a
LIBFT		= $(LIBFTDIR)/libft.a
MLX			= $(MLXDIR)/libmlx.a

# Compiler and flags
CC			= cc
CFLAGS		= -Wall -Wextra -Werror \
			  $(if $(filter 1,$(DEBUG)),-g3) -D DEBUG=$(DEBUG) \
			  -D WIDTH=$(WIDTH) -D HEIGHT=$(HEIGHT) \
			  -std=gnu11
DFLAGS		= -MMD -MP -MF $(DEPDIR)/$*.d
IFLAGS		= -I$(INCDIR) -I$(LIBFTDIR)/include -I$(MLXDIR)
CF			= $(CC) $(CFLAGS) $(IFLAGS) $(DFLAGS)

AR          = $(if $(findstring -flto,$(CC)),llvm-ar-12,ar) $(SILENCE)
ARFLAGS		= rcs
RANLIB      = $(if $(findstring -flto,$(CC)),llvm-ranlib-12,ranlib) $(SILENCE)

# VPATH
vpath %.h $(INCDIR) $(LIBFTDIR)/$(INCDIR) $(MLXDIR)
vpath %.o $(OBJDIR) $(LIBFTDIR)/$(OBJDIR)
vpath %.d $(DEPDIR) $(LIBFTDIR)/$(DEPDIR)

# Sources
MKS			= font_drawer/font_drawer.mk font_filler/font_filler.mk \
			  ttf_parser/ttf_parser.mk text_renderer/text_renderer.mk \
			  mlx_layer/mlx_layer.mk utils/utils.mk

include $(addprefix $(SRCDIR)/, $(MKS))

OBJS		= $(addprefix $(OBJDIR)/, $(notdir $(SRCS:.c=.o)))
DEPS		= $(addprefix $(DEPDIR)/, $(notdir $(SRCS:.c=.d)))

all: $(NAME)

debug: $(NAME)

$(NAME): $(MLX) $(LIBFT) $(OBJS)
	$(call ar-msg)
	@$(AR) $(ARFLAGS) $@ $^
	@$(if $(findstring -flto,$(CC)),$(RANLIB) $@,)
	$(call ar-finish-msg)

$(LIBFT):
	@$(MAKE) -s -C $(LIBFTDIR) $(if $(filter 1,$(DEBUG)),debug)

$(MLX):
	$(call mlx-build-msg)
	@$(MAKE) -s -C $(MLXDIR) $(MUTE)
	$(call mlx-finish-msg)

$(OBJDIR)/%.o: %.c | buildmsg $(OBJDIR) $(DEPDIR)
	$(call lib-compile-obj-msg)
	@$(CF) -c $< -o $@

$(OBJDIR) $(DEPDIR):
	$(call create-dir-msg)
	@mkdir -p $@

buildmsg:
ifneq ($(shell [ -f $(NAME) ] && echo exists),exists)
	$(call lib-build-msg)
endif

help:
	@echo "Available targets:"
	@echo -e "\tall, $(NAME)\t\t: Build the library"
	@echo -e "\tdebug\t\t\t\t: Build the library with debug symbols"
	@echo -e "\tre\t\t\t\t: Rebuild $(NAME)"
	@echo
	@echo -e "\tclean\t\t\t\t: Remove object files"
	@echo -e "\tfclean\t\t\t\t: Remove object files, libraries"
	@echo
	@echo -e "\tprint-%\t\t\t\t: Prints makefile variable content when replacing '%'"

print-% : ; $(info $* is a $(flavor $*) variable set to [$($*)]) @true

clean:
	@$(MAKE) -s -C $(LIBFTDIR) clean
	$(call rm-obj-msg)
	@rm -rf $(OBJDIR) $(DEPDIR)

fclean:
	@$(MAKE) -s -C $(MLXDIR) clean $(SILENCE)
	@$(MAKE) -s -C $(LIBFTDIR) fclean
	$(call rm-obj-msg)
	@rm -rf $(OBJDIR) $(DEPDIR)
	$(call rm-lib-msg)
	@rm -f $(NAME)

re: fclean all

-include $(DEPS)

.PHONY: all debug re clean fclean help buildmsg print-%
