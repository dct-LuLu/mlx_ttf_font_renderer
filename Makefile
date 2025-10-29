# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/11 10:16:04 by jaubry--          #+#    #+#              #
#    Updated: 2025/10/29 02:46:19 by jaubry--         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ROOTDIR		?= .
include $(ROOTDIR)/mkidir/make_utils.mk

# Variables
WINDOWLESS	= 0
FULLSCREEN	= 0
RESIZEABLE	= 0
ifeq ($(FULLSCREEN), 1)
WIDTH		= 1920
HEIGHT		= 1080
else
WIDTH		= 500
HEIGHT		= 500
endif
PERF		= 0

# Directories
CDIR		= font_renderer
SRCDIR		= src
INCDIR		= include
OBJDIR		= .obj
DEPDIR		= .dep

XCERRCALDIR	= $(LIBDIR)/xcerrcal
LIBFTDIR	= $(LIBDIR)/libft
MLXDIR		= $(LIBDIR)/minilibx-linux
MLXWDIR		= $(LIBDIR)/mlx_wrapper

# Output
NAME		= libfont-renderer.a
NAME_TEST	= font_test
NAME_TESTER	= font_tester
XCERRCAL	= $(XCERRCALDIR)/libxcerrcal.a
LIBFT		= $(LIBFTDIR)/libft.a
MLX			= $(MLXDIR)/libmlx.a
MLXW		= $(MLXWDIR)/libmlx-wrapper.a
ARCHIVES	= $(MLXW) $(MLX) $(LIBFT) $(XCERRCAL) $(LIBFT)

# Compiler and flags
CC			= cc

CFLAGS		= -Wall -Wextra -Werror \
			  -std=gnu11

DFLAGS		= -MMD -MP -MF $(DEPDIR)/$*.d

IFLAGS		= -I$(INCDIR) -I$(MLXWDIR)/include -I$(MLXDIR) -I$(XCERRCALDIR)/include -I$(LIBFTDIR)/include

#LFLAGS		= -L$(MLXDIR) -L$(LIBFTDIR) -lXext -lX11 -lXrender -lm -lmlx -lft

LFLAGS		= -L$(CDIR) -L$(MLXWDIR) -L$(MLXDIR) -L$(XCERRCALDIR) -L$(LIBFTDIR) \
			  -lfont-renderer -lmlx-wrapper -lmlx -lft -lxcerrcal \
			  -lXext -lX11 -lXrandr -lm

VARS		= DEBUG=$(DEBUG) \
			  WIDTH=$(WIDTH) \
			  HEIGHT=$(HEIGHT) \
			  PERF=$(PERF) \
			  FULLSCREEN=$(FULLSCREEN) \
			  RESIZEABLE=$(RESIZEABLE) \
			  WINDOWLESS=$(WINDOWLESS)
VFLAGS		= $(addprefix -D ,$(VARS))

CFLAGS		+= $(DEBUG_FLAGS) $(FFLAGS) $(VFLAGS)

CF			= $(CC) $(CFLAGS) $(IFLAGS) $(DFLAGS)

AR          = $(if $(findstring -flto,$(FFLAGS)),$(FAST_AR),$(STD_AR))
ARFLAGS		= rcs
RANLIB      = $(if $(findstring -flto,$(FFLAGS)),$(FAST_RANLIB),$(STD_RANLIB))

# VPATH
vpath %.h $(INCDIR) $(LIBFTDIR)/$(INCDIR) $(MLXWDIR)/$(INCDIR) $(MLXDIR)
vpath %.o $(OBJDIR) $(LIBFTDIR)/$(OBJDIR) $(MLXWDIR)/$(OBJDIR)
vpath %.d $(DEPDIR) $(LIBFTDIR)/$(DEPDIR) $(MLXWDIR)/$(DEPDIR)

# Sources
ifeq ($(if $(filter test $(NAME_TEST),$(MAKECMDGOALS)),1,0),1)
include $(SRCDIR)/test/test.mk
endif

ifeq ($(if $(filter tester $(NAME_TESTER),$(MAKECMDGOALS)),1,0),1)
include $(SRCDIR)/tester/tester.mk
endif

include $(SRCDIR)/srcs.mk


OBJS		= $(addprefix $(OBJDIR)/, $(notdir $(SRCS:.c=.o)))
DEPS		= $(addprefix $(DEPDIR)/, $(notdir $(SRCS:.c=.d)))

all:	$(NAME)
fast:	$(NAME)
debug:	$(NAME)

$(NAME): $(XCERRCAL) $(MLXW) $(MLX) $(LIBFT) $(OBJS)
	$(call ar-msg)
	@$(AR) $(ARFLAGS) $@ $^ $(SILENCE)
ifeq ($(FAST),1)
	@$(RANLIB) $@ $(SILENCE)
endif
	$(call ar-finish-msg)

test: $(NAME_TEST)
$(NAME_TEST): $(NAME)
	$(CF) $(LFLAGS) $(TEST_SRCS) $(ARCHIVES) $(LFLAGS) -o $@

tester: $(NAME_TESTER)
$(NAME_TESTER): $(NAME)
	$(CF) $(LFLAGS) $(TESTER_SRCS) $(ARCHIVES) $(LFLAGS) -o $@

$(XCERRCAL):
	@$(MAKE) -s -C $(XCERRCALDIR) $(RULE) $(VARS) ROOTDIR=../..

$(LIBFT):
	@$(MAKE) -s -C $(LIBFTDIR) $(RULE) $(VARS) ROOTDIR=../..

$(MLXW): $(MLX) $(LIBFT)
	@$(MAKE) -s -C $(MLXWDIR) $(RULE) $(VARS) ROOTDIR=../..

$(MLX):
	$(call mlx-build-msg)
	@$(MAKE) -s -C $(MLXDIR) CC="$(MLX_GCC) $(if $(filter 1,$(FAST)),$(OFLAGS))" $(MUTE)
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
	@$(MAKE) -s -C $(MLXWDIR) clean ROOTDIR=../..
	$(call rm-obj-msg)
	@rm -rf $(OBJDIR) $(DEPDIR)

fclean:
	@$(MAKE) -s -C $(MLXWDIR) fclean ROOTDIR=../..
	$(call rm-obj-msg)
	@rm -rf $(OBJDIR) $(DEPDIR)
	$(call rm-lib-msg)
	@rm -f $(NAME)
	$(call color,$(RED),"Removing program %UL%$(NAME_TEST)")
	@rm -f $(NAME_TEST)
	$(call color,$(RED),"Removing program %UL%$(NAME_TESTER)")
	@rm -f $(NAME_TESTER)

re: fclean all

-include $(DEPS)

.PHONY: all debug test tester re clean fclean help buildmsg print-%
