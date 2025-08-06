/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rasterizer.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 04:21:30 by jaubry--          #+#    #+#             */
/*   Updated: 2025/08/07 01:43:47 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef RASTERIZER_H
# define RASTERIZER_H

# include "libft.h"
# include "mlx_utils.h"
# include "parser_font_ttf.h"
# include "color_utils.h"

# define ON_CURVE 0x01

# define MAX_GLYPH_HEIGHT	3000
# define MAX_ACTIVE_EDGES	550
# define MAX_INTERSECTIONS	30

typedef struct s_text	t_text;

typedef struct s_curve_params
{
	int					contour_start;
	int					contour_idx;
	int					contour_end;
	t_vec2i				start_pt;
	t_vec2i				ctrl_pt;
	t_vec2i				end_pt;
}						t_curve_params;

typedef struct s_curve_state
{
	int					curr_idx;
	int					next_idx;
	int					start_idx;
	int					end_idx;
	t_vec2i				start_pt;
}						t_curve_state;

typedef struct s_contour
{
	t_text				*text;
	t_glyf_table		*glyf;
	int					glyf_idx;
	int					idx;
	t_glyf_component	*transform;
	t_vec2i				pos;
}						t_contour;

typedef struct s_edge
{
	int				ymax;			// Y-coordinate where edge ends
	float			x_current;		// Current x-intersection
	float			inv_slope;		// 1/slope for incremental updates
	int				winding;		// +1 or -1 for winding direction
	struct s_edge	*next;
}					t_edge;

typedef struct s_edge_pool
{
	t_edge			edges[MAX_ACTIVE_EDGES];
	int				free_indices[MAX_ACTIVE_EDGES];
	int				free_count;
}					t_edge_pool;

typedef struct s_fill_data
{
	t_edge			*edge_table[MAX_GLYPH_HEIGHT];// Array indexed by y
	t_edge_pool		edge_pool;
	t_edge			*active_edges;	// Current active edges
	int				intersections[MAX_INTERSECTIONS];
	int				windings[MAX_INTERSECTIONS];
	int				y_min;			// From glyph header
	int				y_max;			// From glyph header
	int				height;			// y_max - y_min + 1
	int				y;
	t_text			*text;
}					t_fill_data;

typedef struct s_text_metrics
{
	int			advance_width;
	int			left_side_bearing;
	t_vec2i		glyph_bbox_min;
	t_vec2i		glyph_bbox_max;
}				t_text_metrics;

t_vec2i	create_implied_point(t_vec2i ctrl1_pt, t_vec2i ctrl2_pt);

void	add_edge(t_fill_data *fill, t_vec2i p1, t_vec2i p2);
void	add_curve_fill(t_fill_data *fill, t_contour *contour,
			t_curve_params params);

t_vec2i	get_transformed_point(t_glyf_table *glyf, int point_idx,
			t_glyf_component *transform);
t_vec2i	apply_transform(t_vec2i point, t_glyf_component *comp);	
t_vec2i	new_screen_pt(t_contour *contour, int x, int y);
t_vec2i	to_screen_pt(t_text *text, t_vec2i glyph_pos, t_vec2i base_pos);
t_vec2i	apply_transform(t_vec2i point, t_glyf_component *comp);
float	scale_x(t_text *text, int _x, int glyph_x);
float	scale_y(t_text *text, int _y, int glyph_y);

t_vec2i	get_glyf_coords(t_glyf_table *glyf, int idx);

void	draw_max_bounding_box(t_contour *contour, int color);
void	draw_transformed_bounding_box(t_contour *contour, int scolor);

#endif//RASTERIZER_H