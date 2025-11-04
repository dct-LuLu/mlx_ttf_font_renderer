/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   font_renderer.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 23:07:22 by jaubry--          #+#    #+#             */
/*   Updated: 2025/11/04 18:27:39 by jaubry--         ###   ########.fr       */
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

# define WHITE	0xFFFFFFFF
# define YELLOW	0xFFFFFF00
# define GREEN	0xFF00FF00
# define RED	0xFFFF0000

# define MAX_TEXTS 2
# define MAX_CHARS 1000

typedef struct s_text
{
	unsigned char	content[MAX_CHARS];
	t_ttf_font		*font;
	t_vec2i			pos;
	t_vec2i			lt;
	t_vec2i			rb;
	unsigned int	font_size;
	t_rgba_int		fg;
	t_rgb_int		bg;
	bool			outlined;
	t_rgba_int		outline;
	bool			subpixel;
	t_img_data		*img;
}					t_text;

int		init_ttf(const char *path, t_ttf_font **ptr);
void	free_ttf(t_ttf_font *font);
void	draw_text(t_text *text);

#endif // FONT_RENDERER_H
