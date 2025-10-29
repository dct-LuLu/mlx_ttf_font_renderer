/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_subpixel_antialiasing.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 15:24:15 by jaubry--          #+#    #+#             */
/*   Updated: 2025/10/29 04:49:31 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"
#include "rasterizer.h"

static int	calculate_coverage(int start, int end, int subpixel_pos)
{
	if ((subpixel_pos < start) || (subpixel_pos > end))
		return (0);
	return (255);
}

static t_rgba_int	blend_subpixels(t_rgb_int bg, t_rgba_int fg,
	int coverage[3])
{
	t_rgba_int	blend;

	blend.r = bg.r + ((fg.r - bg.r) * coverage[0]) / 255;
	blend.g = bg.g + ((fg.g - bg.g) * coverage[1]) / 255;
	blend.b = bg.b + ((fg.b - bg.b) * coverage[2]) / 255;
	blend.a = fg.a;//sure ?
	return (blend);
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
	t_rgba_int	color;

	pixel_x = x[0];
	while (pixel_x <= x[1])
	{
		sub_start = pixel_x * 3;
		coverage[0] = calculate_coverage(sub_x[0], sub_x[1], sub_start);
		coverage[1] = calculate_coverage(sub_x[0], sub_x[1], sub_start + 1);
		coverage[2] = calculate_coverage(sub_x[0], sub_x[1], sub_start + 2);
		apply_lcd_filter(coverage);
		color = blend_subpixels(fill->text->bg, fill->text->fg, coverage);
		ft_mlx_safe_pixel_aput(fill->text->img, vec2i(pixel_x, y), color);
		pixel_x++;
	}
}
