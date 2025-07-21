/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 14:49:14 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/28 04:27:24 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_UTILS_H
# define FILE_UTILS_H

# include <stdint.h>
# include <stdlib.h>

typedef struct s_buffer
{
	uint8_t	*data;					// Entire file content
	size_t	size;					// Total file size
	size_t	pos;					// Current read position
	size_t	pos_id_range_offset;
}			t_buffer;

int			load_file(const char *path, t_buffer **buf);

void		read_bytes(t_buffer *buf, void *dest, size_t n);

#endif // FILE_UTILS_H
