/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   font_renderer.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 23:07:22 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/13 22:32:41 by jaubry--         ###   ########lyon.fr   */
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
	t_vec2				start_pt;
	t_vec2				control_pt;
	t_vec2				end_pt;
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
    int             ymax;           // Y-coordinate where edge ends
    float           x_current;      // Current x-intersection
    float           inv_slope;      // 1/slope for incremental updates
    int             winding;        // +1 or -1 for winding direction
    struct s_edge   *next;
}   t_edge;

typedef struct s_fill_data
{
    t_edge          **edge_table;   // Array of edge lists (indexed by y)
    t_edge          *active_edges;  // Current active edges
    int             y_min;          // From glyph header
    int             y_max;          // From glyph header
    int             height;         // y_max - y_min + 1
}   t_fill_data;


void	*renderer_mainloop(t_env *env);
int		draw_routine(t_env *env);
int		on_keypress(int keysym, t_env *env);
int		mouse_handler(int mousecode, int x, int y, t_env *env);

t_vec2	create_implied_point(t_vec2 control1_pt, t_vec2 control2_pt);

void    move_edges_to_active(t_fill_data *fill, int y);
void    sort_active_edges(t_edge **active_edges);

void    cleanup_fill_data(t_fill_data *fill);

void	fill_glyph(t_contour *contour);

void    process_fill_contour_point(t_fill_data *fill, t_contour *contour,
	int curr_idx, t_curve_params *params, int contour_direction);

void    add_edge(t_fill_data *fill, t_vec2 p1, t_vec2 p2, int contour_direction);
void	add_curve_fill(t_fill_data *fill, t_contour *contour, t_curve_params params, int contour_direction, int depth);
void	process_all_off_curve_contour(t_fill_data *fill, t_contour *contour,
	t_curve_params *params, int contour_direction);

t_vec2	new_screen_pt(t_contour *contour, int x, int y);
t_vec2	apply_transform(t_vec2 point, t_glyf_component *comp);

void	draw_max_bounding_box(t_contour *contour, int color);
void	draw_transformed_bounding_box(t_contour *contour, int scolor);

void	free_env(t_env *env);
#endif // FONT_RENDERER_H
