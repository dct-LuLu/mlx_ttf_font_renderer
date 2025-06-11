# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/11 10:16:04 by jaubry--          #+#    #+#              #
#    Updated: 2025/06/11 16:27:16 by jaubry--         ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

SHELL := /bin/bash

# Print utils
include colors.mk

# Variables
DEBUG		= $(if $(filter debug,$(MAKECMDGOALS)),1,0)
WIDTH		= 500
HEIGHT		= 500

# Directories
SRCDIR		= src
OBJDIR		= .obj
DEPDIR		= .dep
INCDIR		= include
LIBFTDIR	= libft
MLXDIR		= minilibx-linux

# Output
NAME		= font_renderer
LIBFT		= $(LIBFTDIR)/libft.a
MLX			= $(MLXDIR)/libmlx.a

# Compiler and flags
CC			= cc
CFLAGS		= -Wall -Wextra -Werror $(if $(filter 1,$(DEBUG)),-g3) -D DEBUG=$(DEBUG) -D WIDTH=$(WIDTH) -D HEIGHT=$(HEIGHT) -std=gnu11
DFLAGS		= -MMD -MP -MF $(DEPDIR)/$*.d
IFLAGS		= -I$(INCDIR) -I$(LIBFTDIR)/include -I$(MLXDIR)
LFLAGS		= -L$(MLXDIR) -L$(LIBFTDIR) -lXext -lX11 -lXrender -lm -lmlx -lft
CF			= $(CC) $(CFLAGS) $(IFLAGS)

# VPATH
vpath %.c $(SRCDIR)
vpath %.h $(INCDIR) $(LIBFTDIR)/$(INCDIR) $(MLXDIR)
vpath %.o $(OBJDIR) $(LIBFTDIR)/$(OBJDIR)
vpath %.d $(DEPDIR) $(LIBFTDIR)/$(DEPDIR)

# Sources
SRCS		= main.c parser_font_ttf.c \
			  parser_subtable_offset.c parser_subtable_entry.c \
			  parser_table_head.c \
			  parser_table_cmap.c parser_table_cmap_utils.c \
			  parser_table_maxp.c \
			  parser_table_hhea.c \
			  parser_table_hmtx.c \
			  parser_table_loca.c \
			  parser_table_glyf.c parser_table_glyf_header.c \
			  parser_table_glyf_simple.c parser_table_glyf_composite.c parser_table_glyf_utils.c \
			  render_bounding_box.c render_glyph.c render_contour.c render_utils.c \
			  render_curves.c render_curves_utils.c \
			  tag_utils.c \
			  init_utils.c \
			  free_utils1.c free_utils2.c free_utils3.c \
			  file_utils.c \
			  error_handler.c \
			  img_utils.c mlx_draw.c mlx_draw_utils.c mlx_font_renderer.c mlx_draw_curves.c \
			  mlx_hooks.c vec2.c


SRCS		:= $(addprefix $(SRCDIR)/, $(SRCS))

OBJS		= $(addprefix $(OBJDIR)/, $(notdir $(SRCS:.c=.o)))
DEPS		= $(addprefix $(DEPDIR)/, $(notdir $(SRCS:.c=.d)))

all: $(LIBFT) $(NAME)

debug: $(NAME)

$(NAME): $(MLX) $(LIBFT) $(OBJS)
	@$(CF) $^ $(LFLAGS) -o $@
ifeq ($(DEBUG),1)
	$(call color,$(ORANGE)$(BOLD),"✓ Debug build %UL%$(NAME)%NUL% complete")
else
	$(call color,$(GREEN)$(BOLD),"✓ Program %UL%$(NAME)%NUL% successfully created!")
endif

$(LIBFT):
	@$(MAKE) -s -C $(LIBFTDIR) $(if $(filter 1,$(DEBUG)),debug)

$(MLX):
	@echo -e "$(PURPLE)-> Building $(UNDERLINE)minilibx$(RESET)"
	@$(MAKE) -s -C $(MLXDIR)

$(OBJDIR)/%.o: %.c | $(OBJDIR) $(DEPDIR) buildmsg
	$(call color,$(BLUE),"➜ Compiling %UL%$<")
	@$(CF) $(DFLAGS) -c $< -o $@

$(OBJDIR) $(DEPDIR):
	$(call color,$(CYAN),"Creating directory %UL%$@")
	@mkdir -p $@

buildmsg:
ifneq ($(shell [ -f $(NAME) ] && echo exists),exists)
ifeq ($(DEBUG),1)
	$(call color,$(YELLOW)$(BOLD),"$(NL)⚠ Building %UL%$(NAME)%NUL% in debug mode...")
else
	$(call color,$(PURPLE),"$(NL)Creating program %UL%$(NAME)%NUL%...")
endif
endif

help:
	@echo "Available targets:"
	@echo "  all     : Build the program"
	@echo "  clean   : Remove object files"
	@echo "  fclean  : Remove object files and program"
	@echo "  re      : Rebuild everything"
	@echo "  debug   : Build with debug symbols"

print-% : ; $(info $* is a $(flavor $*) variable set to [$($*)]) @true

clean:
	@$(MAKE) -s -C $(LIBFTDIR) clean
	$(call color,$(RED),"Cleaning %UL%$(NAME)%NUL% object files from %UL%$(OBJDIR)%NUL% and %UL%$(DEPDIR)")
	@rm -rf $(OBJDIR) $(DEPDIR)

fclean:
	@$(MAKE) -s -C $(MLXDIR) clean
	@$(MAKE) -s -C $(LIBFTDIR) fclean
	$(call color,$(RED),"Cleaning %UL%$(NAME)%NUL% object files from %UL%$(OBJDIR)%NUL% and %UL%$(DEPDIR)")
	@rm -rf $(OBJDIR) $(DEPDIR)
	$(call color,$(RED),"Removing program %UL%$(NAME)")
	@rm -f $(NAME)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re debug help buildmsg
