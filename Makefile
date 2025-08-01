# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/11 10:16:04 by jaubry--          #+#    #+#              #
#    Updated: 2025/07/21 14:50:18 by jaubry--         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SHELL := /bin/bash

# Print utils
include colors.mk

# Variables
DEBUG		= $(if $(filter debug debug-test debug-tester,$(MAKECMDGOALS)),1,0)
WIDTH		= 500
HEIGHT		= 500

# Directories
ifeq ($(if $(filter tester $(NAME_TESTER) debug-tester, $(MAKECMDGOALS)),1,0),0)
SRCDIR		= src
INCDIR		= include
else
SRCDIR		= src-tester
INCDIR		= include-tester
endif
OBJDIR		= .obj
DEPDIR		= .dep
LIBFTDIR	= libft
MLXDIR		= minilibx-linux

# Output
NAME		= font_renderer.a
NAME_TEST	= test_font_renderer
NAME_TESTER	= tester_font_renderer
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
ifeq ($(if $(filter tester debug-tester $(NAME_TESTER),$(MAKECMDGOALS)),1,0),1)
MKS			= tester.mk
else
MKS			= font_drawer/font_drawer.mk font_filler/font_filler.mk \
			  ttf_parser/ttf_parser.mk text_renderer/text_renderer.mk \
			  mlx_layer/mlx_layer.mk utils/utils.mk \
			  $(if $(filter test debug-test,$(MAKECMDGOALS)),test/test.mk)
endif
include $(addprefix $(SRCDIR)/, $(MKS))



OBJS		= $(addprefix $(OBJDIR)/, $(notdir $(SRCS:.c=.o)))
DEPS		= $(addprefix $(DEPDIR)/, $(notdir $(SRCS:.c=.d)))

all: $(LIBFT) $(NAME)
debug: $(NAME)
$(NAME): $(MLX) $(LIBFT) $(OBJS)
	@$(AR) $(ARFLAGS) $@ $^
ifeq ($(DEBUG), 1)
	$(call color,$(ORANGE)$(BOLD),"✓ %UL%$@%NUL% debug build complete")
else
	$(call color,$(GREEN)$(BOLD),"✓ Library %UL%$@%NUL% successfully created!")
endif

test: $(NAME_TEST)
debug-test: $(NAME_TEST)
$(NAME_TEST): $(MLX) $(LIBFT) $(OBJS)
	@$(CF) $^ $(LFLAGS) -o $@
ifeq ($(DEBUG),1)
	$(call color,$(ORANGE)$(BOLD),"✓ Debug build %UL%$@%NUL% complete")
else
	$(call color,$(GREEN)$(BOLD),"✓ Program %UL%$@%NUL% successfully created!")
endif

tester: $(NAME_TESTER)
debug-tester: $(NAME_TESTER)
$(NAME_TESTER): $(MLX) $(LIBFT) $(OBJS)
	@$(CF) $^ $(LFLAGS) -o $@
ifeq ($(DEBUG),1)
	$(call color,$(ORANGE)$(BOLD),"✓ Debug build %UL%$@%NUL% complete")
else
	$(call color,$(GREEN)$(BOLD),"✓ Program %UL%$@%NUL% successfully created!")
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
ifeq ($(if $(filter all debug $(NAME),$(MAKECMDGOALS)),1,0),1)
ifneq ($(shell [ -f $(NAME) ] && echo exists),exists)
ifeq ($(DEBUG),1)
	$(call color,$(YELLOW)$(BOLD),"$(NL)⚠ Building %UL%$(NAME)%NUL% in debug mode...")
else
	$(call color,$(PURPLE),"$(NL)Creating library %UL%$(NAME)%NUL%...")
endif
endif
endif
ifeq ($(if $(filter test $(NAME_TEST),$(MAKECMDGOALS)),1,0),1)
ifneq ($(shell [ -f $(NAME_TEST) ] && echo exists),exists)
ifeq ($(DEBUG),1)
	$(call color,$(YELLOW)$(BOLD),"$(NL)⚠ Building %UL%$(NAME_TEST)%NUL% in debug mode...")
else
	$(call color,$(PURPLE),"$(NL)Creating program %UL%$(NAME_TEST)%NUL%...")
endif
endif
endif
ifeq ($(if $(filter tester $(NAME_TESTER),$(MAKECMDGOALS)),1,0),1)
ifneq ($(shell [ -f $(NAME) ] && echo exists),exists)
ifeq ($(DEBUG),1)
	$(call color,$(YELLOW)$(BOLD),"$(NL)⚠ Building %UL%$(NAME)%NUL% in debug mode...")
else
	$(call color,$(PURPLE),"$(NL)Creating program %UL%$(NAME)%NUL%...")
endif
endif
endif

help:
	@echo "Available targets:"
	@echo -e "\tall, $(NAME)\t\t: Build the library"
	@echo -e "\tdebug\t\t\t\t: Build the library with debug symbols"
	@echo -e "\tre\t\t\t\t: Rebuild $(NAME)"
	@echo
	@echo -e "\ttest, $(NAME_TEST)\t: Build the test program"
	@echo -e "\tdebug-test\t\t\t: Build the test program with debug symbols"
	@echo
	@echo -e "\ttester, $(NAME_TESTER)\t: Build the font tester program"
	@echo -e "\tdebug-tester\t\t\t: Build the font tester program with debug symbols"
	@echo
	@echo -e "\tclean\t\t\t\t: Remove object files"
	@echo -e "\tfclean\t\t\t\t: Remove object files, libraries and program"
	@echo
	@echo -e "\tprint-%\t\t\t\t: Prints makefile variable content when replacing '%'"

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
	$(call color,$(RED),"Removing library %UL%$(NAME)")
	@rm -f $(NAME)
	$(call color,$(RED),"Removing program %UL%$(NAME_TEST)")
	@rm -f $(NAME_TEST)
	$(call color,$(RED),"Removing program %UL%$(NAME_TESTER)")
	@rm -f $(NAME_TESTER)

re: fclean all

-include $(DEPS)

.PHONY: all debug re test debug-test tester debug-tester clean fclean help buildmsg 
