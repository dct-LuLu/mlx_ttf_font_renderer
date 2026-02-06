/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 22:59:55 by jaubry--          #+#    #+#             */
/*   Updated: 2026/02/06 23:11:51 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "file_utils.h"
#include "libft.h"

void	read_bytes(t_buffer *buf, void *dest, size_t n)
{
	ft_memcpy(dest, buf->data + buf->pos, n);
	buf->pos += n;
}

int16_t	be16toh_signed(int16_t value)
{
	uint16_t tmp;
	
	ft_memcpy(&tmp, &value, sizeof(uint16_t));
	tmp = be16toh(tmp);
	ft_memcpy(&value, &tmp, sizeof(int16_t));
	return (value);
}
