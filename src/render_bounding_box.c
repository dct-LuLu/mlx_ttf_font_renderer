/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_bounding_box.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:45:29 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/10 20:49:35 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

/*
t_vec2	new_screen_pt(t_params *params, int x, int y, t_glyf_component *comp)
{
	t_vec2	pt;
	t_vec2	transformed;
	t_vec2	screen;
	
	pt = new_vec2(x, y);
	if (comp)
	transformed = apply_transform(pt, comp);
	screen = transform_coordinate(params->env, transformed, params->pos);
	return (screen);
}
*/

void	draw_transformed_bounding_box(t_contour *contour, int scolor)
{
	t_vec2			corners[4];
	t_env			*env;
	t_glyf_header	*header;
	t_vec2			pos;
	int				color;

	env = contour->env;
	header = contour->glyf->header;
	pos = contour->pos;
	if (scolor == -1)
		color = contour->color;
	else
		color = scolor;
	corners[0] = transform_coordinate(env, new_vec2(header->x_min, header->y_min), pos);
	corners[1] = transform_coordinate(env, new_vec2(header->x_max, header->y_max), pos);
	corners[2] = transform_coordinate(env, new_vec2(header->x_max, header->y_min), pos);
	corners[3] = transform_coordinate(env, new_vec2(header->x_min, header->y_max), pos);
	ft_mlx_line_put(&env->mlx->img, corners[0], corners[2], color);
	ft_mlx_line_put(&env->mlx->img, corners[0], corners[3], color);
	ft_mlx_line_put(&env->mlx->img, corners[1], corners[2], color);
	ft_mlx_line_put(&env->mlx->img, corners[1], corners[3], color);
}

/**
 * @brief Draw the font-wide max bounding box
 */
void	draw_max_bounding_box(t_contour *contour, int color)
{
	t_vec2			corners[4];
	t_env			*env;
	t_head_table	*head;
	t_vec2			pos;
	int				color;

	env = contour->env;
	head = env->font->head;
	pos = contour->pos;
	corners[0] = transform_coordinate(env, new_vec2(head->x_min, head->y_min), pos);
	corners[1] = transform_coordinate(env, new_vec2(head->x_max, head->y_max), pos);
	corners[2] = transform_coordinate(env, new_vec2(head->x_max, head->y_min), pos);
	corners[3] = transform_coordinate(env, new_vec2(head->x_min, head->y_max), pos);
	ft_mlx_line_put(&env->mlx->img, corners[0], corners[2], color);
	ft_mlx_line_put(&env->mlx->img, corners[0], corners[3], color);
	ft_mlx_line_put(&env->mlx->img, corners[1], corners[2], color);
	ft_mlx_line_put(&env->mlx->img, corners[1], corners[3], color);
}
