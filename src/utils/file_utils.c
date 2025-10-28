/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 13:35:00 by jaubry--          #+#    #+#             */
/*   Updated: 2025/10/12 21:16:54 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "file_utils.h"
#include "frdr_xcerrcal.h"
#include "libft.h"
#include <fcntl.h>
#include <stdlib.h>

static inline void	err_close(int fd)
{
	int	temp;

	temp = errno;
	close(fd);
	errno = temp;
}

static inline int	file_len(const char *path, size_t *total)
{
	uint8_t	temp[4096];
	int		fd;
	ssize_t	read_ret;

	fd = open(path, O_RDONLY);
	if (fd == -1)
	{
		register_complex_err_msg(FRDR_E_MSG_FOPEN, path);
		return (error(pack_err(FRDR_ID, FRDR_E_FOPEN), FL, LN, FC));
	}
	while (true)
	{
		read_ret = read(fd, temp, sizeof(temp));
		if (read_ret < 0)
		{
			err_close(fd);
			register_complex_err_msg(FRDR_E_MSG_FREAD, path);
			return (error(pack_err(FRDR_ID, FRDR_E_FREAD), FL, LN, FC));
		}
		*total += read_ret;
		if (read_ret < 4096)
			break ;
	}
	return (close(fd), 0);
}

static inline int	buffer_file(const char *path, t_buffer **buf)
{
	int	fd;

	fd = open(path, O_RDONLY);
	if (fd == -1)
	{
		register_complex_err_msg(FRDR_E_MSG_FOPEN, path);
		return (error(pack_err(FRDR_ID, FRDR_E_FOPEN), FL, LN, FC));
	}
	if (read(fd, (*buf)->data, (*buf)->size) != (ssize_t)(*buf)->size)
	{
		err_close(fd);
		register_complex_err_msg(FRDR_E_MSG_FREAD, path);
		return (error(pack_err(FRDR_ID, FRDR_E_FREAD), FL, LN, FC));
	}
	return (0);
}

int	load_file(const char *path, t_buffer **buf)
{
	ssize_t	ret;

	*buf = ft_calloc(sizeof(t_buffer), 1);
	if (!*buf)
		return (-1);
	ret = file_len(path, &(*buf)->size);
	if (ret)
		return (error(pack_err(FRDR_ID, FRDR_E_FSIZE), FL, LN, FC));
	(*buf)->data = ft_calloc((*buf)->size, 1);
	if (!(*buf)->data)
		return (-1);
	return (buffer_file(path, buf));
}

void	read_bytes(t_buffer *buf, void *dest, size_t n)
{
	ft_memcpy(dest, buf->data + buf->pos, n);
	buf->pos += n;
}
