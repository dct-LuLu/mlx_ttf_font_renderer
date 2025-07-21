/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 20:14:20 by jaubry--          #+#    #+#             */
/*   Updated: 2025/07/21 12:46:32 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATH_UTILS_H
# define MATH_UTILS_H

# include <math.h>
# include <stdlib.h>
# include "vec2.h"

# define CURVE_RESOLUTION   4.0f

int		fast_distance(t_vec2 a, t_vec2 b);

float	clamp(float value, float min_val, float max_val);

t_vec2	quad_bezier_pt(t_vec2 start, t_vec2 ctrl, t_vec2 end, float t);
float	quad_bezier_res(t_vec2 start, t_vec2 ctrl, t_vec2 end);
float	quad_curve_length(t_vec2 start, t_vec2 ctrl, t_vec2 end);

#endif //MATH_UTILS_H
