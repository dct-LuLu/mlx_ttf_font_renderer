/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_img_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 12:11:45 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/28 02:22:58 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "mlx_utils.h"

/*
	Function to init image with given size, will init it's metadata too.
*/
t_img	init_img(void *mlx, int width, int height)
{
	t_img	img;

	img.img = mlx_new_image(mlx, width, height);
	if (!img.img)
		return (img);
	img.addr = mlx_get_data_addr(img.img, &img.byte_depth, &img.line_len,
			&img.endian);
	img.byte_depth /= 8;
	img.width = width;
	img.height = height;
	return (img);
}

/*
	Function that kills the image
*/
void	kill_img(void *mlx, t_img *img)
{
	if (img->img)
	{
		mlx_destroy_image(mlx, img->img);
		img->img = NULL;
	}
	if (img->addr)
		img->addr = NULL;
	img->byte_depth = 0;
	img->width = 0;
	img->height = 0;
}
