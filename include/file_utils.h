/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 14:49:14 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/15 10:34:28 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_UTILS_H
# define FILE_UTILS_H

# include <stdint.h>

typedef struct s_buffer
{
	uint8_t	*data;						// Entire file content
	size_t	size;						// Total file size
	size_t	pos;						// Current read position
}			t_buffer;

int		load_file(const char *path, t_buffer **buf);

void	read_bytes(t_buffer *buf, void *dest, size_t n);

#endif //FILE_UTILS_H
