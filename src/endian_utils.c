/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   endian_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 11:08:03 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/15 13:55:31 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "endian_utils.h"

/*
	Functiom that do a little test to determine if we're on a little or big
	endian system
*/
int	is_little_endian(void)
{
	uint16_t	test;

	test = 0x0001;
	return (*(uint8_t *)&test == 0x01);
}

/*
	Fonction wrapper for transforming big endian uint32_t to little endian
	Uses inline optimization for remove function call overhead
*/
inline uint32_t	uswap32(uint32_t x)
{
	return (be32toh(x));
}

/*
	Fonction wrapper for transforming big endian uint16_t to little endian
	Uses inline optimization for remove function call overhead
*/
inline uint16_t	uswap16(uint16_t x)
{
	return (be16toh(x));
}

/*
	Fonction wrapper for transforming big endian uint32_t to little endian
	Uses inline optimization for remove function call overhead
*/
inline int32_t	swap32(int32_t x)
{
	return (be32toh(x));
}

/*
	Fonction wrapper for transforming big endian uint16_t to little endian
	Uses inline optimization for remove function call overhead
*/
inline int16_t	swap16(int16_t x)
{
	return (be16toh(x));
}
