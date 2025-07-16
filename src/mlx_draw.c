/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_draw.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:39:37 by jaubry--          #+#    #+#             */
/*   Updated: 2025/07/16 01:52:26 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

void	draw_glyph(t_contour *contour);
void	show_fps(t_env *env);

/**
 * @brief Draw a grid of all available glyphs
 */
static void	draw_char(t_env *env, char c, t_vec2 pos, int color)
{
	t_contour	contour;

	ft_bzero(&contour, sizeof(t_contour));
	contour.env = env;
	contour.color = color;
	contour.pos = pos;
	contour.glyf_idx = get_glyph_index(env->font, c);
	if (contour.glyf_idx >= env->font->maxp->num_glyphs)
		return ;
	if (DEBUG)
		draw_max_bounding_box(&contour, RED);
	draw_glyph(&contour);
}

static void	draw_glyph_grid(t_env *env, int grid_cols, float cell_width,
		float cell_height)
{
	t_contour	contour;
	int			row;
	int			col;

	ft_bzero(&contour, sizeof(t_contour));
	contour.env = env;
	contour.color = WHITE;
	while (contour.glyf_idx < env->font->maxp->num_glyphs)
	{
		row = contour.glyf_idx / grid_cols;
		col = contour.glyf_idx % grid_cols;
		contour.pos = new_vec2(col * cell_width, row * cell_height);
		if (DEBUG)
			draw_max_bounding_box(&contour, RED);
		draw_glyph(&contour);
		contour.glyf_idx++;
	}
}

static void	draw_important_characters(t_env *env)
{
	const char	important[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ\
abcdefghijklmnopqrstuvwxyz0123456789";
	size_t		i;
	int			cols;
	float		cell_width;
	float		cell_height;

	cols = 16;
	cell_width = abs(env->font->head->x_min) + env->font->head->x_max;
	cell_height = abs(env->font->head->y_min) + env->font->head->y_max;
	i = 0;
	while (important[i])
	{
		draw_char(env, important[i],
			new_vec2((i % cols) * cell_width, (i / cols) * cell_height), WHITE);
		i++;
	}
}

static void	draw_text_example(t_env *env)
{
	t_vec2	pos;

	pos = new_vec2(500, 500);
	draw_string(env, env->text, pos, WHITE);
}

int	draw_routine(t_env *env)
{
	t_mlx		*mlx;

	mlx = env->mlx;
	mlx->tick += 1;
	ft_mlx_batch_put(&mlx->img, mlx->origin, mlx->size, BACKGROUND);
	if (env->view_mode == 0)
		draw_glyph_grid(env, 16, abs(env->font->head->x_min)
			+ env->font->head->x_max, abs(env->font->head->y_min)
			+ env->font->head->y_max);
	else if (env->view_mode == 1)
		draw_important_characters(env);
	else if (env->view_mode == 2)
		draw_text_example(env);
	show_fps(env);
	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->img.img, 0, 0);
	return (0);
}
