/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   endian_utils.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 11:53:24 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/15 13:54:57 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENDIAN_UTILS_H
# define ENDIAN_UTILS_H
# include <endian.h>
# include <stdint.h>

int				is_little_endian(void);


//inline uint32_t	swap32(uint32_t val);
//uint32_t	swap32(uint32_t val);
extern uint32_t	uswap32(uint32_t val);
extern int32_t	swap32(int32_t val);

//inline uint16_t	swap16(uint16_t val);
//uint16_t	swap16(uint16_t val);
extern uint16_t	uswap16(uint16_t val);
extern int16_t	swap16(int16_t val);

#endif //ENDIAN_UTILS_H
