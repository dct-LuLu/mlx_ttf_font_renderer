/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   font_renderer.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 23:07:22 by jaubry--          #+#    #+#             */
/*   Updated: 2025/08/21 18:56:31 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef FONT_RENDERER_H
# define FONT_RENDERER_H

# include "rasterizer.h"

# ifndef DEBUG
#  define DEBUG 0
# endif//DEBUG

# ifndef WIDTH
#  define WIDTH 500
# endif//WIDTH

# ifndef HEIGHT
#  define HEIGHT 500
# endif//HEIGHT

# ifndef PERF
#  define PERF 0
# endif//PERF

# ifndef FULLSCREEN
#  define FULLSCREEN 0
# endif//FULLSCREEN

# ifndef WINDOWLESS
#  define WINDOWLESS 0
# endif//WINDOWLESS

# ifndef RESIZEABLE
#  define RESIZEABLE 0
# endif//RESIZEABLE

# define WHITE	0xFFFFFF
# define YELLOW	0xFFFF00
# define GREEN	0x00FF00
# define RED	0xFF0000

# define MAX_TEXTS 100
# define MAX_CHARS 10

typedef struct s_text
{
	char			content[MAX_CHARS];
	t_ttf_font		*font;
	t_vec2i			pos;
	unsigned int	size;
	uint32_t		fg;
	uint32_t		bg;
	bool			outlined;
	uint32_t		outline;
	bool			subpixel;
	t_img_data		*img;
}					t_text;

typedef struct s_rast_env
{
	t_mlx	*mlx;
	t_text	*texts[MAX_TEXTS];
	size_t	text_num;
	t_text	*fps;
}	t_rast_env;

int		init_ttf(const char *path, t_ttf_font **ptr);
void	free_ttf(t_ttf_font *font);
void	draw_text(t_text *text);

#endif // FONT_RENDERER_H
