/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_draw.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:39:37 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/17 01:38:55 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

void rasterize_glyph(t_env *env, unsigned char ch, t_vec2 pos, int color)
{
	t_glyf_table	*glyph;

	glyph = env->font->glyfs[ch];
	if (!glyph)
		return ;
	//if (!glyph)
	//	return ;
	//printf("index: %zu\n", get_glyph_index(env->font, ch));
	//printf("glyph: %p\n", glyph);
    // For each contour in the glyph
    int start_pt = 0;
    //printf("glyph->header: %p\n", glyph->header);
    for (int c = 0; c < glyph->header->number_of_contours; c++) {
        int end_pt = glyph->end_pts[c];
        
        // Process each point in the contour
        for (int i = start_pt; i <= end_pt; i++) {
            int next_i = (i == end_pt) ? start_pt : i + 1;
            
            int	div = 10;
            // Draw line between current point and next point
    		t_vec2 a = new_vec2(glyph->x_coordinates[i] / div + pos.x, glyph->y_coordinates[i] / div+ pos.y);
    		t_vec2 b = new_vec2(glyph->x_coordinates[next_i] /div+ pos.x, glyph->y_coordinates[next_i] /div + pos.y);
            //    glyph->flags[i] & 0x01  // On-curve flag
            ft_mlx_line_put(&env->mlx->img, a, b, color);
        }
        start_pt = end_pt + 1;
    }
    
    // Fill the outline
    //fill_outline(bitmap);
}

int	draw_routine(t_env *env)
{
	t_mlx	*mlx;
	size_t	i;

	mlx = env->mlx;
	mlx->tick += 1;
	ft_mlx_batch_put(&mlx->img, mlx->origin, mlx->size, BACKGROUND);
	i = 0;
	while (i < 256)
	{
		rasterize_glyph(env, i, new_vec2(-500 + i *75, 100), 0x0000FF19);
		i++;
	}
	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->img.img, 0, 0);
	return (0);
}
