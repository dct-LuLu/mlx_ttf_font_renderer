/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   font_renderer.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 23:07:22 by jaubry--          #+#    #+#             */
/*   Updated: 2025/07/14 16:51:15 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef FONT_RENDERER_H
# define FONT_RENDERER_H

# include "libft.h"
# include "mlx_utils.h"
# include "parser_font_ttf.h"

# define WHITE	0xFFFFFF
# define YELLOW	0xFFFF00
# define GREEN	0x00FF00
# define RED	0xFF0000

# define ON_CURVE 0x01

# define MAX_GLYPH_HEIGHT	3000
# define MAX_GLYPH_WIDTH	2500
# define MAX_ACTIVE_EDGES	550
# define MAX_INTERSECTIONS	30

# define CURVE_RESOLUTION   4.0f

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

typedef struct s_subpixel_data
{
	float		coverage[MAX_GLYPH_WIDTH * 3];
	float		filtered[MAX_GLYPH_WIDTH * 3];
	int			bg_color;
	int			gamma_to_linear[256];
	int			linear_to_gamma[256];
	int			display_start;
	int			display_end;
	int			subpixel_start;
	int			subpixel_end;
	int32_t		bg;
	int32_t		fg;
	uint32_t	diff;
	uint8_t		rdiff;
	uint8_t		gdiff;
	uint8_t		bdiff;
}				t_subpixel_data;

typedef struct s_env
{
	t_mlx				*mlx;
	t_ttf_font			*font;
	char				text[200];
	size_t				cur_pos;
	bool				capslock;
	bool				subpixel;
	int					x;
	int					y;
	int					last_x;
	int					last_y;
	int					zoom;
	int					view_mode;
}						t_env;

typedef struct s_contour
{
	t_glyf_table		*glyf;
	int					glyf_idx;
	int					idx;
	t_vec2				pos;
	int					color;
	t_glyf_component	*transform;
	t_env				*env;
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
	int				color;
	t_env			*env;
}					t_fill_data;

typedef struct s_text_metrics
{
	int			advance_width;
	int			left_side_bearing;
	t_vec2		glyph_bbox_min;
	t_vec2		glyph_bbox_max;
}				t_text_metrics;

//void	draw_string_right_aligned(t_env *env, const char *str, t_vec2 right_pos,
//	int color);
//void	draw_string_centered(t_env *env, const char *str, t_vec2 center_pos,
//		int color);

//t_text_metrics	get_glyph_metrics(t_env *env, size_t glyph_idx);
//int				measure_string_width(t_env *env, const char *str);

int		save_env_state(t_env *env, const char *filename);
int		load_env_state(t_env *env, const char *filename);

void	draw_string(t_env *env, const char *str, t_vec2 pos, int color);

void	*renderer_mainloop(t_env *env);
int		draw_routine(t_env *env);
int		on_button_press(int mousecode, int x, int y, t_env *env);
int		on_mwheel_drag(int x, int y, t_env *env);
int		on_button_release(int mousecode, int x, int y, t_env *env);
int		on_key_press(int keysym, t_env *env);
int		mouse_handler(int mousecode, int x, int y, t_env *env);

t_vec2	create_implied_point(t_vec2 ctrl1_pt, t_vec2 ctrl2_pt);

void	add_edge(t_fill_data *fill, t_vec2 p1, t_vec2 p2);
void	add_curve_fill(t_fill_data *fill, t_contour *contour,
			t_curve_params params);

t_vec2	new_screen_pt(t_contour *contour, int x, int y);
t_vec2	apply_transform(t_vec2 point, t_glyf_component *comp);
float	transform_x(t_contour *contour, int _x);
int		transform_y(t_contour *contour, int _y);
void	subpixelize(t_vec2 *pos);
t_vec2	new_screen_pt2(t_contour *contour, t_vec2 v);
t_vec2	get_glyf_coords(t_glyf_table *glyf, int idx);

void	draw_max_bounding_box(t_contour *contour, int color);
void	draw_transformed_bounding_box(t_contour *contour, int scolor);

void	free_env(t_env *env);
#endif // FONT_RENDERER_H
