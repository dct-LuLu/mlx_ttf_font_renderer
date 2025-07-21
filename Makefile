# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/11 10:16:04 by jaubry--          #+#    #+#              #
#    Updated: 2025/07/21 12:42:34 by jaubry--         ###   ########.fr        #
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
CFLAGS		= -Ofast -Wall -Wextra -Werror \
			  $(if $(filter 1,$(DEBUG)),-g3) -D DEBUG=$(DEBUG) \
			  -D WIDTH=$(WIDTH) -D HEIGHT=$(HEIGHT) \
			  -std=gnu11
DFLAGS		= -MMD -MP -MF $(DEPDIR)/$*.d
IFLAGS		= -I$(INCDIR) -I$(LIBFTDIR)/include -I$(MLXDIR)
LFLAGS		= -L$(MLXDIR) -L$(LIBFTDIR) -lXext -lX11 -lXrender -lm -lmlx -lft
CF			= $(CC) $(CFLAGS) $(IFLAGS)

AR			= ar
ARFLAGS		= rcs

# VPATH
vpath %.h $(INCDIR) $(LIBFTDIR)/$(INCDIR) $(MLXDIR)
vpath %.o $(OBJDIR) $(LIBFTDIR)/$(OBJDIR)
vpath %.d $(DEPDIR) $(LIBFTDIR)/$(DEPDIR)

# Sources
MKS			= font_drawer/font_drawer.mk font_filler/font_filler.mk \
			  ttf_parser/ttf_parser.mk text_renderer/text_renderer.mk \
			  mlx_layer/mlx_layer.mk utils/utils.mk \
			  test/test.mk

include $(addprefix $(SRCDIR)/, $(MKS))

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
