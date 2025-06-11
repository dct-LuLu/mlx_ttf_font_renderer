/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   font_renderer.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 23:07:22 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/11 23:33:32 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef FONT_RENDERER_H
# define FONT_RENDERER_H

# include "libft.h"
# include "mlx_utils.h"
# include "parser_font_ttf.h"

# define WHITE 0x00FFFFFF
# define YELLOW 0x00FFFF00
# define GREEN 0x0000FF00
# define RED 0x00FF0000

# define ON_CURVE 0x01

typedef struct s_curve_params
{
	int					contour_start;
	int					contour_end;
}						t_curve_params;

typedef struct s_curve_state
{
	int					curr_idx;
	int					next_idx;
	int					start_idx;
	int					end_idx;
	t_vec2				start_pt;
}						t_curve_state;

typedef struct s_env
{
	t_mlx				*mlx;
	t_ttf_font			*font;
	int					x;
	int					y;
	float				zoom;
	int					view_mode;
}						t_env;

typedef struct s_contour
{
	t_glyf_table		*glyf;
	int					glyf_idx;
	int					idx;
	t_vec2				pos;
	int					color;
	t_glyf_component	*transform;//? sure
	t_env				*env;
}						t_contour;

typedef struct s_edge
{
	int				ymin;
	int				ymax;
	float			x_current;
	float			dx;
	int				winding;
	struct s_edge	*next;
}						t_edge;

typedef struct s_fill_context
{
	t_glyf_table		*glyf;
	t_edge				*aet;
	int					scanline_y;
	int					fill_color;
	t_vec2				position;
	t_glyf_component	*transform;
}						t_fill_context;

void	*renderer_mainloop(t_env *env);
int		draw_routine(t_env *env);
int		on_keypress(int keysym, t_env *env);
int		mouse_handler(int mousecode, int x, int y, t_env *env);

t_vec2	new_screen_pt(t_contour *contour, int x, int y);
t_vec2	apply_transform(t_vec2 point, t_glyf_component *comp);

void	draw_max_bounding_box(t_contour *contour, int color);
void	draw_transformed_bounding_box(t_contour *contour, int scolor);

void	free_env(t_env *env);
#endif // FONT_RENDERER_H
