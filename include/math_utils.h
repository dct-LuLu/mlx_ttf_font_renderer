/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 20:14:20 by jaubry--          #+#    #+#             */
/*   Updated: 2025/07/16 20:14:21 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATH_UTILS_H
# define MATH_UTILS_H

# include <math.h>
# include <stdlib.h>
# include "vec2.h"

int		fast_distance(t_vec2 a, t_vec2 b);

float	clamp(float value, float min_val, float max_val);

#endif //MATH_UTILS_H