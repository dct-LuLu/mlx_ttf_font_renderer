/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_routine.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:39:37 by jaubry--          #+#    #+#             */
/*   Updated: 2025/11/04 16:53:39 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"
#include "tester.h"

void	update_fps(t_env *env);
void	draw_glyph(t_contour *contour);

/**
 * @brief Draw a grid of all available glyphs
 */
static void	draw_char(t_env *env, char c, t_vec2i pos, t_rgba_int color)
{
	t_contour	contour;

	ft_bzero(&contour, sizeof(t_contour));
	contour.glyf_idx = get_glyph_index(env->font, c);
	if (contour.glyf_idx >= env->font->maxp->num_glyphs)
		return ;
	contour.text = &(t_text)
	{
		.font_size = env->zoom,
		.fg = color,
		.outlined = false,
		.subpixel = false,
		.font = env->font,
		.img = &(env->mlx->img)
	};
	contour.pos = to_screen_pt(contour.text, pos, vec2i(env->x, env->y));
	if (DEBUG)
		draw_max_bounding_box(&contour, (t_rgba_int){.rgba=RED});
	draw_glyph(&contour);
}

static void	draw_glyph_grid(t_env *env, int grid_cols, float cell_width,
		float cell_height)
{
	t_contour	contour;
	int			row;
	int			col;

	ft_bzero(&contour, sizeof(t_contour));
	while (contour.glyf_idx < env->font->maxp->num_glyphs)
	{
		row = contour.glyf_idx / grid_cols;
		col = contour.glyf_idx % grid_cols;
		contour.text = &(t_text)
		{
			.font_size = env->zoom,
			.fg = (t_rgba_int){.rgba=WHITE},
			.outlined = false,
			.subpixel = true,
			.font = env->font,
			.img = &(env->mlx->img)
		};
		contour.pos = to_screen_pt(contour.text, vec2i(col * cell_width, -row * cell_height), vec2i(env->x, env->y));
		if (DEBUG)
			draw_max_bounding_box(&contour, (t_rgba_int){.rgba=RED});
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
	printf("w: %f, h: %f\n", cell_width, cell_height);
	i = 0;
	while (important[i])
	{
		draw_char(env, important[i],
			
			vec2i((i % cols) * cell_width, (i / cols) * cell_height), (t_rgba_int){.rgba=WHITE});
		i++;
	}
}

int	draw_routine(t_env *env)
{
	t_mlx	*mlx;

	mlx = env->mlx;
	ft_mlx_batch_put(&mlx->img, mlx->origin, mlx->size, (t_rgb_int){.rgb=BACKGROUND});
	if (env->view_mode == 0)
		draw_glyph_grid(env, 16, abs(env->font->head->x_min)
			+ env->font->head->x_max, abs(env->font->head->y_min)
			+ env->font->head->y_max);
	else if (env->view_mode == 1)
		draw_important_characters(env);
	else if (env->view_mode == 2)
		draw_text(env->text);
	env->text->_text_pos = vec2i(env->x, env->y);
	env->text->font_size = env->zoom;
	//draw_texts(env);
	if (env->subpixel)
	{
		env->text->outlined = true;
		env->text->fg = (t_rgba_int){.rgba=0};
	}
	else
	{
		env->text->outlined = false;
		env->text->fg = (t_rgba_int){.rgba=WHITE};
	}
	update_fps(env);
	draw_text(env->fps);
	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->img.img, 0, 0);
	return (0);
}
