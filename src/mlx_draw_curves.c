/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_draw_curves.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:19:25 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/26 16:00:42 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx_utils.h"
#include <math.h>

static t_vec2 quadratic_bezier_point(t_vec2 p0, t_vec2 p1, t_vec2 p2, float t)
{
    float m = 1.0f - t;
    t_vec2 result;

    result.x = m * m * p0.x + 2 * m * t * p1.x + t * t * p2.x;
    result.y = m * m * p0.y + 2 * m * t * p1.y + t * t * p2.y;
    return (result);
}

static float calculate_curve_length(t_vec2 p0, t_vec2 p1, t_vec2 p2)
{
    // Approximate curve length for adaptive step sizing
    float chord_length = sqrt((p2.x - p0.x) * (p2.x - p0.x) + 
                             (p2.y - p0.y) * (p2.y - p0.y));
    float control_length = sqrt((p1.x - p0.x) * (p1.x - p0.x) + 
                               (p1.y - p0.y) * (p1.y - p0.y)) +
                          sqrt((p2.x - p1.x) * (p2.x - p1.x) + 
                               (p2.y - p1.y) * (p2.y - p1.y));
    
    return (chord_length + control_length) * 0.5f;
}

void ft_mlx_draw_quadratic_curve(t_img *img, t_vec2 p0, t_vec2 p1, t_vec2 p2, int color)
{
    float curve_length = calculate_curve_length(p0, p1, p2);
    float step = 2.0f / (curve_length + 1.0f);  // Adaptive step size
    t_vec2 current, previous;
    float t;
    
    // Ensure minimum quality
    if (step > 0.1f)
        step = 0.1f;
    if (step < 0.01f)
        step = 0.01f;
    
    previous = p0;
    t = step;
    
    while (t <= 1.0f)
    {
        current = quadratic_bezier_point(p0, p1, p2, t);
        ft_mlx_line_put(img, previous, current, color);
        previous = current;
        t += step;
    }
    
    // Ensure we end exactly at p2
    ft_mlx_line_put(img, previous, p2, color);
}

