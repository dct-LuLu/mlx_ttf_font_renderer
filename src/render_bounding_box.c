/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_bounding_box.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:45:29 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/14 22:43:57 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

/*
	Draw the glyph max bounding box
*/
void	draw_transformed_bounding_box(t_contour *contour, int scolor)
{
	t_vec2			corners[4];
	t_glyf_header	*header;
	int				color;

	header = contour->glyf->header;
	if (scolor == -1)
		color = contour->color;
	else
		color = scolor;
	corners[0] = new_screen_pt(contour, header->x_min, header->y_min);
	corners[1] = new_screen_pt(contour, header->x_max, header->y_max);
	corners[2] = new_screen_pt(contour, header->x_max, header->y_min);
	corners[3] = new_screen_pt(contour, header->x_min, header->y_max);
	ft_mlx_line_put(&contour->env->mlx->img, corners[0], corners[2], color);
	ft_mlx_line_put(&contour->env->mlx->img, corners[0], corners[3], color);
	ft_mlx_line_put(&contour->env->mlx->img, corners[1], corners[2], color);
	ft_mlx_line_put(&contour->env->mlx->img, corners[1], corners[3], color);
}

/*
	Draw the font-wide max bounding box
*/
void	draw_max_bounding_box(t_contour *contour, int color)
{
	t_vec2			corners[4];
	t_env			*env;
	t_head_table	*head;
	//t_vec2			pos;

	env = contour->env;
	head = env->font->head;
	//pos = contour->pos;
	corners[0] = new_screen_pt(contour, head->x_min, head->y_min);
	corners[1] = new_screen_pt(contour, head->x_max, head->y_max);
	corners[2] = new_screen_pt(contour, head->x_max, head->y_min);
	corners[3] = new_screen_pt(contour, head->x_min, head->y_max);
	ft_mlx_line_put(&env->mlx->img, corners[0], corners[2], color);
	ft_mlx_line_put(&env->mlx->img, corners[0], corners[3], color);
	ft_mlx_line_put(&env->mlx->img, corners[1], corners[2], color);
	ft_mlx_line_put(&env->mlx->img, corners[1], corners[3], color);
}
