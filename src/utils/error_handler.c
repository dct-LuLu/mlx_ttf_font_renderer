/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 09:27:08 by jaubry--          #+#    #+#             */
/*   Updated: 2025/08/20 21:39:53 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error_handler.h"
#include "libft.h"
#include <stdarg.h>
#include <string.h>

static const char	*get_err_msg(enum e_error err)
{
	if ((int)err == errno)
		return (strerror(errno));
	if (err == ERR_ARG_NUM)
		return (ERR_MSG_ARG_NUM);
	else if (err == ERR_FILE_EXT)
		return (ERR_MSG_FILE_EXT);
	else if (err == ERR_CORRUPTED_FONT)
		return (ERR_MSG_CORRUPTED_FONT);
	else if (err == ERR_OFFSET_SUBTABLE)
		return (ERR_MSG_OFFSET_SUBTABLE);
	else if (err == ERR_FONT_TYPE)
		return (ERR_MSG_FONT_TYPE);
	else if (err == ERR_READ_FILE)
		return (ERR_MSG_READ_FILE);
	else if (err == ERR_GET_OFFSET)
		return (ERR_MSG_GET_OFFSET);
	else if (err == ERR_TIME)
		return (ERR_MSG_TIME);
	return (strerror(errno));
}

int	rerror(enum e_error err, const char *format, ...)
{
	int		errno_cpy;
	va_list	args;

	errno_cpy = errno;
	if (err)
		ft_dprintf(2, "rerror [%d]: ", err);
	ft_dprintf(2, "%s", get_err_msg(err));
	if (DEBUG && format)
	{
		ft_dprintf(2, " - ");
		va_start(args, format);
		ft_vdprintf(2, format, args);
		va_end(args);
	}
	ft_dprintf(2, "\n");
	if (!err)
		return (errno_cpy);
	return (err);
}
