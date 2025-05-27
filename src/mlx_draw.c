/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_draw.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:39:37 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/27 23:25:21 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"





/**
 * @brief Draw a grid of all available glyphs
 */
void	draw_glyph_grid(t_env *env, int grid_cols, float cell_width,
					float cell_height)
{
	size_t	glyph_index;
	t_vec2	cell_pos;
	int	row;
	int	col;

	glyph_index = 0;
	while (glyph_index < env->font->maxp->num_glyphs)
	{
		row = glyph_index / grid_cols;
		col = glyph_index % grid_cols;

		// Fixed cell position - always at top-left corner
		cell_pos = new_vec2(col * cell_width, row * cell_height);

		// Draw both bounding boxes
		if (DEBUG)
			draw_max_bounding_box(env, cell_pos, RED);  // Red for max bounding box
		draw_glyph_outline(env, glyph_index, cell_pos, WHITE);

		glyph_index++;
	}
}

void	draw_important_characters(t_env *env)
{
	char	important[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	size_t	i;
	int		cols;
	float	cell_width;
	float	cell_height;
	
	cols = 16;
	cell_width = abs(env->font->head->x_min) + env->font->head->x_max;
	cell_height = abs(env->font->head->y_min) + env->font->head->y_max;

	i = 0;
	while (important[i])
	{
		//size_t glyph_index = get_glyph_index(env->font, i);
		int row = i / cols;
		int col = i % cols;
		t_vec2 pos = new_vec2(col * cell_width, row * cell_height);
		size_t	idx = get_glyph_index(env->font, important[i]);	
		draw_max_bounding_box(env, pos, RED);
		draw_glyph_outline(env, idx, pos, WHITE);
		
		// Draw index number for debugging
		char str[16];
		snprintf(str, 16, "%zu", idx);
		i++;
	}
}

void	debug_character_mappings(t_env *env)
{
	int	base_chars[] = {'A', 'Z', 'a', 'z', '0', '9', 0};
	int	i;

	i = 0;
	while (base_chars[i] != 0)
	{
		size_t glyph_index = get_glyph_index(env->font, base_chars[i]);
		printf("Char '%c' (0x%02X) maps to glyph index: %zu\n",
				base_chars[i], base_chars[i], glyph_index);
		i++;
	}

	for (int ch = 0; ch < 128; ch++)
	{
		size_t idx = get_glyph_index(env->font, ch);
		if (idx > 0)
		{
			printf("ASCII %d (0x%02X) -> glyph index %zu\n", ch, ch, idx);
		}
	}
}

int	draw_routine(t_env *env)
{
	t_mlx	*mlx;
	static int	debug_done = 1;

	mlx = env->mlx;
	mlx->tick += 1;
	ft_mlx_batch_put(&mlx->img, mlx->origin, mlx->size, BACKGROUND);

	if (!debug_done)
	{
		debug_character_mappings(env);
		debug_done = 1;
	}
	if (env->view_mode == 0)
		draw_glyph_grid(env, 16, abs(env->font->head->x_min) + env->font->head->x_max, abs(env->font->head->y_min) + env->font->head->y_max);
	else if (env->view_mode == 1)
		draw_important_characters(env);
	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->img.img, 0, 0);
	return (0);
}
