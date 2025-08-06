/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 20:14:20 by jaubry--          #+#    #+#             */
/*   Updated: 2025/08/06 09:09:11 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATH_UTILS_H
# define MATH_UTILS_H

# include <math.h>
# include <stdlib.h>
# include "vectors.h"

# define CURVE_RESOLUTION   4.0f

int		fast_distance(t_vec2i a, t_vec2i b);

float	clamp(float value, float min_val, float max_val);

t_vec2i	quad_bezier_pt(t_vec2i start, t_vec2i ctrl, t_vec2i end, float t);
float	quad_bezier_res(t_vec2i start, t_vec2i ctrl, t_vec2i end);
float	quad_curve_length(t_vec2i start, t_vec2i ctrl, t_vec2i end);

#endif //MATH_UTILS_H
