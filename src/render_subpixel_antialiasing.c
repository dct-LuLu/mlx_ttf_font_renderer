/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_subpixel_antialiasing.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 15:24:15 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/30 17:25:06 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

//inline
uint32_t	to_color(uint8_t red, uint8_t green, uint8_t blue)
{
	return ((red << 16) | (green << 8) | blue);
}

static	uint32_t	compute_start_coverage(t_subpixel_data data)
{
	uint8_t	red_cov;
	uint8_t	green_cov;
	uint8_t	blue_cov;

	red_cov = 0;
	green_cov = 0;
	blue_cov = 0;
	if (data.subpixel_start <= 0)
		red_cov = data.rdiff;
	if (data.subpixel_start <= 1)
		green_cov = data.gdiff;
	if (data.subpixel_start <= 2)
		blue_cov = data.bdiff;
	return (to_color(red_cov, green_cov, blue_cov));
}

static	uint32_t	compute_end_coverage(t_subpixel_data data)
{
	uint8_t	red_cov;
	uint8_t	green_cov;
	uint8_t	blue_cov;

	red_cov = 0;
	green_cov = 0;
	blue_cov = 0;
	if (data.subpixel_end <= 0)
		red_cov = data.rdiff;
	if (data.subpixel_end <= 1)
		green_cov = data.gdiff;
	if (data.subpixel_end <= 2)
		blue_cov = data.bdiff;
	return (to_color(red_cov, green_cov, blue_cov));
}

static t_subpixel_data	init_subpixel_data(t_fill_data *fill, int *x)
{
	t_subpixel_data	data;

	data.display_start = x[0] / 3;
	data.display_end = (x[1] + 2) / 3;
	data.subpixel_start = x[0] % 3;
	data.subpixel_end = x[1] % 3;
	data.bg = 0;//fill->bg_color;
	data.fg = fill->color;
	data.diff = abs(data.bg - data.fg);
	data.rdiff = data.diff >> 16;
	data.gdiff = (data.diff >> 8) & 0xFF;
	data.bdiff = data.diff & 0xFF;
	return (data);
}

void	render_sb(t_fill_data *fill, t_contour *contour, int *x)
{
	t_subpixel_data	data;
	int				xx[2];
	int				yy;

	
	data = init_subpixel_data(fill, x);
	
	xx[0] = transform_x(contour, data.display_start);
	xx[1] = transform_x(contour, data.display_end);
	yy = transform_y(contour, fill->y);
	//ft_mlx_circle_put(&fill->env->mlx->img, new_vec2(xx[0], yy), 5, YELLOW);
	//ft_mlx_circle_put(&fill->env->mlx->img, new_vec2(xx[1], yy), 5, YELLOW);
	ft_mlx_horizontal_line(&fill->env->mlx->img, xx, yy, fill->color);
	if (data.subpixel_start > 0)
		ft_mlx_pixel_put(&fill->env->mlx->img, new_vec2(transform_x(contour, data.display_start), yy), compute_start_coverage(data));
	if (data.subpixel_end > 0)
		ft_mlx_pixel_put(&fill->env->mlx->img, new_vec2(transform_x(contour, data.display_end), yy), compute_end_coverage(data));
}

static int	calculate_coverage(int start, int end, int subpixel_pos)
{
	if (subpixel_pos < start || subpixel_pos > end)
		return (0);
	return (255);
}

static int	blend_subpixels(int bg, int fg, int coverage[3])
{
	int	r, g, b;
	int	bg_r, bg_g, bg_b;
	int	fg_r, fg_g, fg_b;

	bg_r = (bg >> 16) & 0xFF;
	bg_g = (bg >> 8) & 0xFF;
	bg_b = bg & 0xFF;
	fg_r = (fg >> 16) & 0xFF;
	fg_g = (fg >> 8) & 0xFF;
	fg_b = fg & 0xFF;
	r = bg_r + ((fg_r - bg_r) * coverage[0]) / 255;
	g = bg_g + ((fg_g - bg_g) * coverage[1]) / 255;
	b = bg_b + ((fg_b - bg_b) * coverage[2]) / 255;
	return ((r << 16) | (g << 8) | b);
}

static void	apply_lcd_filter(int coverage[3])
{
	int	total;
	int	filtered[3];
	
	total = coverage[0] + coverage[1] + coverage[2];
	filtered[0] = (coverage[0] * 2 + coverage[1]) / 3;
	filtered[1] = (coverage[0] + coverage[1] * 2 + coverage[2]) / 4;
	filtered[2] = (coverage[1] + coverage[2] * 2) / 3;
	int new_total = filtered[0] + filtered[1] + filtered[2];
	if (new_total > 0)
	{
		coverage[0] = (filtered[0] * total) / new_total;
		coverage[1] = (filtered[1] * total) / new_total;  
		coverage[2] = (filtered[2] * total) / new_total;
	}
}

void	render_fb(t_fill_data *fill, int *x, int y)
{
	int	pixel_x;
	int	subpixel_start;
	int	subpixel_end;
	int	coverage[3];
	int	color;
	t_subpixel_data	data;
	data = init_subpixel_data(fill, x);

	pixel_x =  x[0] / 3;
	while (pixel_x <= x[1] / 3)
	{
		subpixel_start = pixel_x * 3;
		subpixel_end = subpixel_start + 2;
		coverage[0] = calculate_coverage(x[0], x[1], subpixel_start);
		coverage[1] = calculate_coverage(x[0], x[1], subpixel_start + 1);
		coverage[2] = calculate_coverage(x[0], x[1], subpixel_start + 2);
		apply_lcd_filter(coverage);
		color = blend_subpixels(data.bg, data.fg, coverage);
		ft_mlx_pixel_put(&fill->env->mlx->img, new_vec2(pixel_x, y), color);
		pixel_x++;
	}
}

void	render_subpixel_line(t_fill_data *fill, t_contour *contour, int *x)
{
	int	xx[2];
	int	yy;

	if (false)
		render_sb(fill, contour, x);
	else
	{
		yy = transform_y(contour, fill->y);
		xx[0] = transform_x(contour, x[0]) * 3;
		xx[1] = transform_x(contour, x[1]) * 3;
		render_fb(fill, xx, yy);
	}
}
