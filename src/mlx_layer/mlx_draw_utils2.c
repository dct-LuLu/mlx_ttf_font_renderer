/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_draw_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 21:10:43 by jaubry--          #+#    #+#             */
/*   Updated: 2025/08/07 01:44:10 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "mlx_utils.h"
#include <math.h>

void	ft_mlx_horizontal_line(t_img_data *img, int xpt[2], int y, int color)
{
	int				start_x;
	int				end_x;
	int				width;
	unsigned int	*pixel_row;
	unsigned int	*end_ptr;

	if ((y < 0) || (y >= img->height))
		return ;
	start_x = xpt[!(xpt[0] < xpt[1])];
	end_x = xpt[(xpt[0] < xpt[1])];
	if (start_x < 0)
		start_x = 0;
	if (end_x >= img->width)
		end_x = img->width - 1;
	width = end_x - start_x + 1;
	if (width <= 0)
		return ;
	pixel_row = (unsigned int *)(img->addr + (y * img->line_len) + (start_x
				* img->byte_depth));
	end_ptr = pixel_row + width;
	while (pixel_row < end_ptr)
	{
		*pixel_row = (unsigned int)color;
		pixel_row++;
	}
}

void	ft_mlx_circle_put(t_img_data *img, t_vec2i center, int radius,
			int color)
{
	int	x;
	int	y;
	int	distance;

	y = -radius;
	while (y <= radius)
	{
		x = -radius;
		while (x <= radius)
		{
			distance = (x * x) + (y * y);
			if (distance <= (radius * radius))
				ft_mlx_pixel_put(img, vec2i(center.x + x, center.y + y),
					color);
			x++;
		}
		y++;
	}
}
