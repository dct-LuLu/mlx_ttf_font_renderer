/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 13:35:00 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/11 13:52:17 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error_handler.h"
#include "file_utils.h"
#include "libft.h"
#include <fcntl.h>
#include <stdlib.h>

static int	file_len(const char *path, size_t *total)
{
	uint8_t	temp[4096];
	int		fd;
	ssize_t	read_ret;

	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (close(fd), error(errno, ": file_len %s", path));
	while (true)
	{
		read_ret = read(fd, temp, sizeof(temp));
		if (read_ret < 0)
			return (close(fd), error(errno, ": file_len %s", path));
		*total += read_ret;
		if (read_ret < 4096)
			break ;
	}
	return (close(fd), 0);
}

int	load_file(const char *path, t_buffer **buf)
{
	ssize_t	ret;
	int		fd;

	*buf = ft_calloc(sizeof(t_buffer), 1);
	if (!*buf)
		return (error(errno, ": load_file buf"));
	ret = file_len(path, &(*buf)->size);
	if (ret)
		return (ret);
	(*buf)->data = ft_calloc((*buf)->size, 1);
	if (!(*buf)->data)
		return (error(errno, ": load_file data"));
	fd = open(path, O_RDONLY);
	if (fd == -1)
	{
		ret = errno;
		return (close(fd), error(ret, ": load_file %s", path));
	}
	ret = read(fd, (*buf)->data, (*buf)->size);
	if ((size_t)ret != (*buf)->size)
		return (close(fd), error(ERR_READ_FILE, ": load_file %s %d %d", path,
				ret, (*buf)->size));
	return (0);
}

void	read_bytes(t_buffer *buf, void *dest, size_t n)
{
	ft_memcpy(dest, buf->data + buf->pos, n);
	buf->pos += n;
}
