/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_draw_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 01:06:14 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/28 02:23:16 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "mlx_utils.h"
#include <math.h>

void	ft_mlx_batch_put(t_img *img, t_vec2 pos, t_vec2 size, int color)
{
	int				x;
	int				y;
	int				offset;
	unsigned int	*pixels;
	size_t			i;

	offset = ((pos.y * img->line_len) + (pos.x * img->byte_depth));
	pixels = (unsigned int *)(img->addr + offset);
	x = 0;
	while (x < size.x)
	{
		y = 0;
		while (y < size.y)
		{
			i = x + y * img->line_len / img->byte_depth;
			pixels[i] = color;
			y++;
		}
		x++;
	}
}

/*
	Function that draws a pixel directly on the address of the img
	on the memory.
*/
void	ft_mlx_pixel_put(t_img *img, t_vec2 pos, int color)
{
	int				offset;
	unsigned int	*pixel;

	if ((pos.x >= 0) && (pos.x < img->width) && (pos.y >= 0)
		&& (pos.y < img->height))
	{
		offset = ((pos.y * img->line_len) + (pos.x * img->byte_depth));
		pixel = (unsigned int *)(img->addr + offset);
		*pixel = color;
	}
}

/*
	Function that initializes the necessary data for the line drawing function
*/
static t_line	get_line_data(t_vec2 a, t_vec2 b)
{
	t_line	line;

	line.dx = abs(b.x - a.x);
	line.dy = -abs(b.y - a.y);
	line.sx = (b.x > a.x) - (b.x < a.x);
	line.sy = (b.y > a.y) - (b.y < a.y);
	line.err = line.dx + line.dy;
	return (line);
}

/*
	Function for line_put logic, to increment the current pixel draw pos.
*/
static void	incr_line(int *pos, int *err, int d, int s)
{
	*err += d;
	*pos += s;
}

/*
	Function that draws a line from point a to point b using a color.
*/
void	ft_mlx_line_put(t_img *img, t_vec2 a, t_vec2 b, int color)
{
	t_line	line;

	line = get_line_data(a, b);
	while (true)
	{
		if ((a.x >= 0) && (a.x <= img->width) && (a.y >= 0)
			&& (a.y <= img->height))
			ft_mlx_pixel_put(img, a, color);
		if ((a.x == b.x) && (a.y == b.y))
			break ;
		line.e2 = line.err * 2;
		if (line.e2 >= line.dy)
		{
			if (a.x == b.x)
				break ;
			incr_line(&a.x, &line.err, line.dy, line.sx);
		}
		if (line.e2 <= line.dx)
		{
			if (a.y == b.y)
				break ;
			incr_line(&a.y, &line.err, line.dx, line.sy);
		}
	}
}
