/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_utils.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 17:19:56 by jaubry--          #+#    #+#             */
/*   Updated: 2025/07/16 17:21:01 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLOR_UTILS_H
# define COLOR_UTILS_H

# include <stdint.h>

# define R 0
# define G 1
# define B 2

uint32_t	to_color(uint8_t red, uint8_t green, uint8_t blue);

#endif //COLOR_UTILS_H