/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_subpixel_antialiasing.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 15:24:15 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/28 22:23:10 by jaubry--         ###   ########lyon.fr   */
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
	if (data.subpixel_end >= 2)
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

void	render_subpixel_line(t_fill_data *fill, t_contour *contour, int *x)
{
	t_subpixel_data	data;
	int				xx[2];
	int				yy;

	
	data = init_subpixel_data(fill, x);
	
	xx[0] = transform_x(contour, data.display_start);
	xx[1] = transform_x(contour, data.display_end);
	yy = transform_y(contour, fill->y);
	ft_mlx_horizontal_line(&fill->env->mlx->img, xx, yy, fill->color);
	if (data.subpixel_start > 0)
		ft_mlx_pixel_put(&fill->env->mlx->img, new_vec2(transform_x(contour, data.display_start), yy), compute_start_coverage(data));
	if (data.subpixel_end > 0)
		ft_mlx_pixel_put(&fill->env->mlx->img, new_vec2(transform_x(contour, data.display_end), yy), compute_end_coverage(data));
}
