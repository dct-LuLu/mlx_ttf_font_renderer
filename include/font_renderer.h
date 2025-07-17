/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   font_renderer.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 23:07:22 by jaubry--          #+#    #+#             */
/*   Updated: 2025/07/17 23:19:15 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef FONT_RENDERER_H
# define FONT_RENDERER_H

# include "libft.h"
# include "mlx_utils.h"
# include "parser_font_ttf.h"
# include "color_utils.h"

# define WHITE	0xFFFFFF
# define YELLOW	0xFFFF00
# define GREEN	0x00FF00
# define RED	0xFF0000

# define ON_CURVE 0x01

# define MAX_GLYPH_HEIGHT	3000
# define MAX_ACTIVE_EDGES	550
# define MAX_INTERSECTIONS	30

# define CURVE_RESOLUTION   4.0f

# define MAX_TEXTS 100
# define MAX_CHARS 10

typedef struct s_text
{
	char			content[MAX_CHARS];
	t_ttf_font		*font;
	t_vec2			pos;
	unsigned int	size;
	uint32_t		fg;
	uint32_t		bg;
	bool			outlined;
	uint32_t		outline;
	bool			subpixel;
	t_img			*img;
}					t_text;

typedef struct s_rast_env
{
	t_mlx	*mlx;
	t_text	*texts[MAX_TEXTS];
	size_t	text_num;
	t_text	*fps;
}	t_rast_env;

typedef struct s_curve_params
{
	int					contour_start;
	int					contour_idx;
	int					contour_end;
	t_vec2				start_pt;
	t_vec2				ctrl_pt;
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

typedef struct s_contour
{
	t_text				*text;
	t_glyf_table		*glyf;
	int					glyf_idx;
	int					idx;
	t_glyf_component	*transform;
	t_vec2				pos;
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
	t_vec2		glyph_bbox_min;
	t_vec2		glyph_bbox_max;
}				t_text_metrics;

t_vec2	create_implied_point(t_vec2 ctrl1_pt, t_vec2 ctrl2_pt);

void	add_edge(t_fill_data *fill, t_vec2 p1, t_vec2 p2);
void	add_curve_fill(t_fill_data *fill, t_contour *contour,
			t_curve_params params);

t_vec2	get_transformed_point(t_glyf_table *glyf, int point_idx,
			t_glyf_component *transform);
t_vec2	apply_transform(t_vec2 point, t_glyf_component *comp);	
t_vec2	new_screen_pt(t_contour *contour, int x, int y);
t_vec2	to_screen_pt(t_text *text, t_vec2 glyph_pos, t_vec2 base_pos);
t_vec2	apply_transform(t_vec2 point, t_glyf_component *comp);
float	scale_x(t_text *text, int _x, int glyph_x);
float	scale_y(t_text *text, int _y, int glyph_y);

t_vec2	get_glyf_coords(t_glyf_table *glyf, int idx);

void	draw_max_bounding_box(t_contour *contour, int color);
void	draw_transformed_bounding_box(t_contour *contour, int scolor);

#endif // FONT_RENDERER_H
