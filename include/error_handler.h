/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 09:10:13 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/11 16:17:09 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_HANDLER_H
# define ERROR_HANDLER_H

# include <errno.h>

# define ERR_MSG_ARG_NUM "Wrong argument number"
# define ERR_MSG_FILE_EXT "Wrong file extension.\nOnly .ttf files are\
supported currently"
# define ERR_MSG_CORRUPTED_FONT "Can't read file's offset table,\
file may bee corrupted"
# define ERR_MSG_OFFSET_SUBTABLE "Invalid font offset table values,\
file may be corrupted"
# define ERR_MSG_FONT_TYPE "Invalid snft version,\
font isn't in a TTF format or file may be corrupted"
# define ERR_MSG_READ_FILE "Error while buffering file"
# define ERR_MSG_GET_OFFSET "Could not fetch table offset"

typedef enum e_error
{
	ERR,
	ERR_ARG_NUM,
	ERR_FILE_EXT,
	ERR_CORRUPTED_FONT,
	ERR_OFFSET_SUBTABLE,
	ERR_FONT_TYPE,
	ERR_TABLE_ENTRIES,
	ERR_READ_FILE,
	ERR_GET_OFFSET,
}	t_error;

int	error(enum e_error err, const char *format, ...);

#endif // ERROR_HANDLER_H
