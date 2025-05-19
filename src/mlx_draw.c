/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_draw.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:39:37 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/19 19:50:24 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

/**
 * @brief Convert font coordinates to screen coordinates
 */
t_vec2	transform_coordinate(t_env *env, t_vec2 glyph_pos, t_vec2 base_pos)
{
	float	scale;
	float	x;
	float	y;

	scale = 500.0f / (float)env->font->head->units_per_em;
	x = (glyph_pos.x * scale) + base_pos.x + env->x;
	y = (-glyph_pos.y * scale) + base_pos.y + env->y;
	return (new_vec2(x / env->zoom, y / env->zoom));
}

/**
 * @brief Get character width based on hmtx table
 */
float	get_glyph_advance(t_ttf_font *font, size_t glyph_index)
{
	if (!font || !font->hmtx || glyph_index >= font->hmtx->num_lhmtx)
		return (600.0f);
	return ((float)font->hmtx->lhmtx[glyph_index].advance_width);
}

/**
 * @brief Draw the font-wide max bounding box
 */
void	draw_max_bounding_box(t_env *env, t_vec2 pos, int color)
{
	t_vec2	lb;
	t_vec2	rb;
	t_vec2	lt;
	t_vec2	rt;
	t_head_table	*head;

	head = env->font->head;
	lb = transform_coordinate(env, new_vec2(head->x_min, head->y_min), pos);
	rt = transform_coordinate(env, new_vec2(head->x_max, head->y_max), pos);
	rb = transform_coordinate(env, new_vec2(head->x_max, head->y_min), pos);
	lt = transform_coordinate(env, new_vec2(head->x_min, head->y_max), pos);

	ft_mlx_line_put(&env->mlx->img, lb, rb, color);
	ft_mlx_line_put(&env->mlx->img, lb, lt, color);
	ft_mlx_line_put(&env->mlx->img, rt, rb, color);
	ft_mlx_line_put(&env->mlx->img, rt, lt, color);
}

/**
 * @brief Draw a glyph outline
 */
int	draw_glyph_outline(t_env *env, size_t glyph_index, t_vec2 pos, int color)
{
	t_glyf_table	*glyph;
	int		contour_index;
	int		point_index;
	int		start_pt;
	int		end_pt;

	glyph = env->font->glyfs[glyph_index];
	if (!glyph || glyph->header->number_of_contours <= 0)
		return (0);

	start_pt = 0;
	contour_index = 0;
	while (contour_index < glyph->header->number_of_contours)
	{
		end_pt = glyph->end_pts[contour_index];
		point_index = start_pt;
		while (point_index <= end_pt)
		{
			int next_point = (point_index == end_pt) ? start_pt : point_index + 1;
			t_vec2 curr_pt = new_vec2(glyph->x_coordinates[point_index],
							glyph->y_coordinates[point_index]);
			t_vec2 next_pt = new_vec2(glyph->x_coordinates[next_point],
							glyph->y_coordinates[next_point]);

			ft_mlx_line_put(&env->mlx->img,
					transform_coordinate(env, curr_pt, pos),
					transform_coordinate(env, next_pt, pos), color);
			point_index++;
		}
		start_pt = end_pt + 1;
		contour_index++;
	}
	return (1);
}

/**
 * @brief Draw the glyph-specific bounding box
 */
void	draw_glyph_bounding_box(t_env *env, size_t glyph_index,
						t_vec2 pos, int color)
{
	t_glyf_table	*glyph;
	t_vec2		lb;
	t_vec2		rb;
	t_vec2		lt;
	t_vec2		rt;

	glyph = env->font->glyfs[glyph_index];
	if (!glyph)
		return ;

	lb = transform_coordinate(env, new_vec2(glyph->header->x_min,
						glyph->header->y_min), pos);
	rt = transform_coordinate(env, new_vec2(glyph->header->x_max,
						glyph->header->y_max), pos);
	rb = transform_coordinate(env, new_vec2(glyph->header->x_max,
						glyph->header->y_min), pos);
	lt = transform_coordinate(env, new_vec2(glyph->header->x_min,
						glyph->header->y_max), pos);

	ft_mlx_line_put(&env->mlx->img, lb, rb, color);
	ft_mlx_line_put(&env->mlx->img, lb, lt, color);
	ft_mlx_line_put(&env->mlx->img, rt, rb, color);
	ft_mlx_line_put(&env->mlx->img, rt, lt, color);
}

/**
 * @brief Draw both max and glyph bounding boxes for a glyph
 */
void	draw_both_bounding_boxes(t_env *env, size_t glyph_index, t_vec2 pos)
{
	draw_max_bounding_box(env, pos, 0x00FF0000);  // Red for max bounding box
	draw_glyph_bounding_box(env, glyph_index, pos, 0x0000FF00);  // Green for glyph bounding box
}

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
		draw_both_bounding_boxes(env, glyph_index, cell_pos);

		if ((glyph_index >= 239 && glyph_index <= 268) || (glyph_index >= 88 && glyph_index <= 130))
			draw_glyph_outline(env, glyph_index, cell_pos, 0x0000FF00);
		else if (glyph_index >= 566 && glyph_index <= 575)  // 0-9 range
			draw_glyph_outline(env, glyph_index, cell_pos, 0x0000FFFF);
		else
			draw_glyph_outline(env, glyph_index, cell_pos, 0x00FFFFFF);

		glyph_index++;
	}
}

/**
 * @brief Draw a specific set of important characters
 */
void	draw_important_characters(t_env *env)
{
	// Characters to draw (A-Z, a-z, 0-9)
	char	important[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	int	i;
	int	cols;
	float	cell_width;
	float	cell_height;
	
	cols = 16;
	cell_width = 600.0f;
	cell_height = 1000.0f;
	
	i = 0;
	while (important[i])
	{
		size_t glyph_index = get_glyph_index(env->font, important[i]);
		int row = i / cols;
		int col = i % cols;
		t_vec2 pos = new_vec2(col * cell_width, row * cell_height);
		
		// Draw highlight box
		draw_both_bounding_boxes(env, glyph_index, pos);
		
		// Draw the glyph in bright green
		draw_glyph_outline(env, glyph_index, pos, 0x0000FF00);
		
		// Draw index number for debugging
		char str[16];
		snprintf(str, 16, "%zu", glyph_index);
		// A pseudo function to display text (you'll need to implement this)
		// display_debug_text(env, str, new_vec2(pos.x, pos.y + cell_height - 50));
		
		i++;
	}
}

/**
 * @brief Debug character mapping
 */
void	debug_character_mappings(t_env *env)
{
	// Test key character mappings for A-Z and a-z
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

	// Check first 128 mappings
	for (int ch = 0; ch < 128; ch++)
	{
		size_t idx = get_glyph_index(env->font, ch);
		if (idx > 0)
		{
			printf("ASCII %d (0x%02X) -> glyph index %zu\n", ch, ch, idx);
		}
	}
}

/**
 * @brief Main drawing routine
 */
int	draw_routine(t_env *env)
{
	t_mlx	*mlx;
	static int	debug_done = 0;
	static int	view_mode = 2;

	mlx = env->mlx;
	mlx->tick += 1;
	ft_mlx_batch_put(&mlx->img, mlx->origin, mlx->size, BACKGROUND);

	if (!debug_done)
	{
		debug_character_mappings(env);
		debug_done = 1;
	}

	// Toggle between views when a key is pressed (you'll need to implement this)
	// view_mode = (env->key_pressed) ? (view_mode + 1) % 3 : view_mode;

	if (view_mode == 0)
	{
		// Draw all glyphs in a grid with aligned cells
		draw_glyph_grid(env, 16, env->font->head->x_max, env->font->head->y_max);  // Reduced height to make grid more square
	}
	else if (view_mode == 1)
	{
		// Draw only important characters (A-Z, a-z, 0-9)
		draw_important_characters(env);
	}
	else
	{
		// Draw specific test characters
		//size_t a_idx = get_glyph_index(env->font, 'A');  // Should be 239
		size_t z_idx = get_glyph_index(env->font, 'Z');  // Should be 268
		
		draw_both_bounding_boxes(env, 'A', new_vec2(300, 300));
		draw_glyph_outline(env, 'A', new_vec2(300, 300), 0x0000FF00);
		
		draw_both_bounding_boxes(env, z_idx, new_vec2(1100, 300));
		draw_glyph_outline(env, z_idx, new_vec2(1100, 300), 0x0000FF00);
	}

	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->img.img, 0, 0);
	return (0);
}

/*
void rasterize_glyph(t_env *env, size_t ch, t_vec2 pos, int color)
{
	t_glyf_table	*glyph;
	int16_t			c;
	int				i;

	glyph = env->font->glyfs[ch];
	if (!glyph)
		return ;
    int start_pt = 0;
    c = 0;
    while (c < glyph->header->number_of_contours)
    {
        int end_pt = glyph->end_pts[c];
        i = start_pt;
        while (i <= end_pt)
        {
            int next_i = (i == end_pt) ? start_pt : i + 1;
            t_vec2 a = new_vec2(glyph->x_coordinates[i], glyph->y_coordinates[i]);
    		t_vec2 b = new_vec2(glyph->x_coordinates[next_i], glyph->y_coordinates[next_i]);
            //    glyph->flags[i] & 0x01  // On-curve flag
            ft_mlx_line_put(&env->mlx->img, process_pos(env, a, pos), process_pos(env, b, pos), color);
            i++;
        }
        start_pt = end_pt + 1;
        c++;
    }
}

static void	draw_bounds(t_env *env, size_t ch, t_vec2 pos, int color)
{
	t_glyf_table	*glyf;
	t_vec2			lb;
	t_vec2			rb;
	t_vec2			lt;
	t_vec2			rt;

	t_head_table	*head = env->font->head;
	lb = process_pos(env, new_vec2(head->x_min, head->y_min), pos);
	rt = process_pos(env, new_vec2(head->x_max, head->y_max), pos);
	rb = process_pos(env, new_vec2(head->x_max, head->y_min), pos);
	lt = process_pos(env, new_vec2(head->x_min, head->y_max), pos);
	ft_mlx_line_put(&env->mlx->img, lb, rb, color);
	ft_mlx_line_put(&env->mlx->img, lb, lt, color);
	ft_mlx_line_put(&env->mlx->img, rt, rb, color);
	ft_mlx_line_put(&env->mlx->img, rt, lt, color);
	glyf = env->font->glyfs[ch];
	if (!glyf)
		return ;
	lb = process_pos(env, new_vec2(glyf->header->x_min, glyf->header->y_min), pos);
	rt = process_pos(env, new_vec2(glyf->header->x_max, glyf->header->y_max), pos);
	rb = process_pos(env, new_vec2(glyf->header->x_max, glyf->header->y_min), pos);
	lt = process_pos(env, new_vec2(glyf->header->x_min, glyf->header->y_max), pos);
	color -= 1200;
	ft_mlx_line_put(&env->mlx->img, lb, rb, color);
	ft_mlx_line_put(&env->mlx->img, lb, lt, color);
	ft_mlx_line_put(&env->mlx->img, rt, rb, color);
	ft_mlx_line_put(&env->mlx->img, rt, lt, color);

}

int	draw_routine(t_env *env)
{
	t_mlx	*mlx;
	size_t	i;

	mlx = env->mlx;
	mlx->tick += 1;
	ft_mlx_batch_put(&mlx->img, mlx->origin, mlx->size, BACKGROUND);
	i = 0;
	//rasterize_glyph(env, 'a', new_vec2(WIDTH / 2, HEIGHT / 2), 0x0000FF19);
	while (i < env->font->maxp->num_glyphs)
	{
		draw_bounds(env, i, new_vec2(((i + 1) % 25) * 600, (i / 25) * 1200), 0x00FF1447);
		rasterize_glyph(env, i, new_vec2(((i + 1) % 25) * 600, (i / 25) * 1200), 0x0000FF19);
		i++;
	}
	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->img.img, 0, 0);
	return (0);
}
*/
