/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_subpixel_antialiasing.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 15:24:15 by jaubry--          #+#    #+#             */
/*   Updated: 2025/07/17 22:27:25 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

static int	calculate_coverage(int start, int end, int subpixel_pos)
{
	if ((subpixel_pos < start) || (subpixel_pos > end))
		return (0);
	return (255);
}

static uint32_t	blend_subpixels(uint32_t bg_color, uint32_t fg_color,
	int coverage[3])
{
	uint8_t	blend[3];
	uint8_t	bg[3];
	uint8_t	fg[3];

	bg[R] = (bg_color >> 16) & 0xFF;
	bg[G] = (bg_color >> 8) & 0xFF;
	bg[B] = bg_color & 0xFF;
	fg[R] = (fg_color >> 16) & 0xFF;
	fg[G] = (fg_color >> 8) & 0xFF;
	fg[B] = fg_color & 0xFF;
	blend[R] = bg[R] + ((fg[R] - bg[R]) * coverage[0]) / 255;
	blend[G] = bg[G] + ((fg[G] - bg[G]) * coverage[1]) / 255;
	blend[B] = bg[B] + ((fg[B] - bg[B]) * coverage[2]) / 255;
	return (to_color(blend[R], blend[G], blend[B]));
}

static void	apply_lcd_filter(int coverage[3])
{
	int	total;
	int	new_total;
	int	filtered[3];

	total = coverage[0] + coverage[1] + coverage[2];
	filtered[0] = ((coverage[0] * 2) + coverage[1]) / 3;
	filtered[1] = (coverage[0] + (coverage[1] * 2) + coverage[2]) / 4;
	filtered[2] = (coverage[1] + (coverage[2] * 2)) / 3;
	new_total = filtered[0] + filtered[1] + filtered[2];
	if (new_total > 0)
	{
		coverage[0] = (filtered[0] * total) / new_total;
		coverage[1] = (filtered[1] * total) / new_total;
		coverage[2] = (filtered[2] * total) / new_total;
	}
}

void	render_subpixel_line(t_fill_data *fill, int *sub_x, int *x, int y)
{
	int	sub_start;
	int	coverage[3];
	int	pixel_x;
	int	color;

	pixel_x = x[0];
	while (pixel_x <= x[1])
	{
		sub_start = pixel_x * 3;
		coverage[0] = calculate_coverage(sub_x[0], sub_x[1], sub_start);
		coverage[1] = calculate_coverage(sub_x[0], sub_x[1], sub_start + 1);
		coverage[2] = calculate_coverage(sub_x[0], sub_x[1], sub_start + 2);
		apply_lcd_filter(coverage);
		color = blend_subpixels(fill->text->bg, fill->text->fg, coverage);
		ft_mlx_pixel_put(fill->text->img, new_vec2(pixel_x, y), color);
		pixel_x++;
	}
}
