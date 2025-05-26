/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   font_renderer.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 23:07:22 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/26 18:37:18 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef FONT_RENDERER_H
# define FONT_RENDERER_H

# include "parser_font_ttf.h"
# include "mlx_utils.h"
# include "libft.h"

# define WHITE 0x00FFFFFF
# define YELLOW 0x00FFFF00
# define GREEN 0x0000FF00
# define RED 0x00FF0000

# define ON_CURVE 0x01

typedef struct s_env
{
	t_mlx		*mlx;
	t_ttf_font	*font;
	int			x;
	int			y;
	float		zoom;
	int			view_mode;
}	t_env;

void	*renderer_mainloop(t_env *env);
int		draw_routine(t_env *env);

int		on_keypress(int keysym, t_env *env);
int		mouse_handler(int mousecode, int x, int y, t_env *env);

t_vec2	transform_coordinate(t_env *env, t_vec2 glyph_pos, t_vec2 base_pos);
t_vec2 	apply_transform(t_vec2 point, t_glyf_component *comp);
t_vec2	get_component_position(t_vec2 base_pos, t_glyf_component *comp);

void	draw_glyph_outline(t_env *env, size_t glyph_index, t_vec2 pos, int color);

void	draw_curve_from_on_curve(t_env *env, t_glyf_table *glyph, int start_idx,
	int ctr_start_idx, int ctr_end_idx, t_vec2 pos, int color,
	t_glyf_component *transform);

void	draw_curve_sequence(t_env *env, t_glyf_table *glyph, int start_idx,
	int end_idx, int ctr_start_idx, int ctr_end_idx, t_vec2 pos, int color,
	t_glyf_component *transform);

void	draw_transformed_bounding_box(t_env *env, t_glyf_header *header, t_vec2 pos, int color, t_glyf_component *comp);
void	draw_max_bounding_box(t_env *env, t_vec2 pos, int color);


void	free_env(t_env *env);
#endif //FONT_RENDERER_H
