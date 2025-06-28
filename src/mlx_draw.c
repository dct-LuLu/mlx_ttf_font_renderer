/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_draw.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:39:37 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/28 13:33:01 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

void	draw_glyph(t_contour *contour);

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

void	draw_glyph_grid(t_env *env, int grid_cols, float cell_width,
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

void	draw_important_characters(t_env *env)
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

void	debug_character_mappings(t_env *env)
{
	const char	base_chars[] = "AZaz09";
	size_t		glyph_index;
	size_t		idx;
	int			i;
	int			ch;

	i = 0;
	while (base_chars[i] != 0)
	{
		glyph_index = get_glyph_index(env->font, base_chars[i]);
		printf("Char '%c' (0x%02X) maps to glyph index: %zu\n", base_chars[i],
			base_chars[i], glyph_index);
		i++;
	}
	ch = 0;
	while (ch < 128)
	{
		idx = get_glyph_index(env->font, ch);
		if (idx > 0)
		{
			printf("ASCII %d (0x%02X) -> glyph index %zu\n", ch, ch, idx);
		}
		ch++;
	}
}

static void	draw_text_example(t_env *env)
{
	t_vec2	pos;

	pos = new_vec2(500, 500);
	draw_string(env, env->text, pos, WHITE);
}

ssize_t get_current_time(void);

int	get_fps(void)
{
	static ssize_t	t1 = -1;
	static ssize_t	t2 = -1;

	t1 = t2;
	t2 = get_current_time();
	return (1000 / (t2 - t1));
}

static size_t	ft_itoalen(long int nb)
{
	size_t	len;

	if (nb == 0)
		return (1);
	len = 0;
	if (nb < 0)
	{
		nb *= -1;
		len++;
	}
	while (nb > 0)
	{
		nb /= 10;
		len++;
	}
	return (len);
}

void	ft_itoal(int n, char *str)
{
	long int	num;
	size_t		i;
	size_t		len;

	i = 0;
	num = (long int)n;
	len = ft_itoalen(num);
	str[0] = '0';
	if (num < 0)
	{
		str[0] = '-';
		num *= -1;
	}
	while (num > 0)
	{
		str[len - i - 1] = '0' + (num % 10);
		num /= 10;
		i++;
	}
}

void	show_fps(t_env *env)
{
	static char	fps[5];
	const t_vec2 pos = new_vec2(200, 200);

	ft_itoal(get_fps(), fps);
	draw_string(env, fps, pos, RED);
}

int	draw_routine(t_env *env)
{
	t_mlx		*mlx;
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
