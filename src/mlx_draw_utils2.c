/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_draw_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 21:10:43 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/12 23:00:15 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "mlx_utils.h"
#include <math.h>

void ft_mlx_horizontal_line(t_img *img, int x1, int x2, int y, int color)
{
    int start_x;
    int end_x;
    int x;

    if (y < 0 || y >= img->height)
        return;
    
    start_x = (x1 < x2) ? x1 : x2;
    end_x = (x1 < x2) ? x2 : x1;
    
    if (start_x < 0)
        start_x = 0;
    if (end_x >= img->width)
        end_x = img->width - 1;
    
    x = start_x;
    while (x <= end_x)
    {
        ft_mlx_pixel_put(img, new_vec2(x, y), color);
        x++;
    }
}

void	ft_mlx_circle_put(t_img *img, t_vec2 center, int radius, int color)
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
			{
				ft_mlx_pixel_put(img, new_vec2(center.x + x, center.y + y), color);
			}
			x++;
		}
		y++;
	}
}
