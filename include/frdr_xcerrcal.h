/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frdr_xcerrcal.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 19:03:07 by jaubry--          #+#    #+#             */
/*   Updated: 2025/10/12 21:11:58 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRDR_XCERRCAL_H
# define FRDR_XCERRCAL_H

# define FRDR_ID 4

typedef enum e_frdr_err
{
	FRDR_E_TTF = 1,
	FRDR_E_EXT,
	FRDR_E_SUBTABLE,
	FRDR_E_TABLE,
	FRDR_E_RTABLE,
	FRDR_E_TTFSTRUCT,
	FRDR_E_SUBSTRUCT,
	FRDR_E_ALLOC,
	FRDR_E_FETCH,
	FRDR_E_FBUFF,
	FRDR_E_FOPEN,
	FRDR_E_FREAD,
	FRDR_E_FSIZE,
	FRDR_E_MTABLE,
	FRDR_E_SFNT,
	FRDR_E_TAG,
	FRDR_E_ENTRIES,
	FRDR_E_OFFSET,
	FRDR_E_FCMAP,
	FRDR_ERRS_NUM
}	t_frdr_err;

#define FRDR_E_MSG_TTF "Font renderer initialization failed for font `%s`"
#define FRDR_E_MSG_EXT "Wrong file extension.\nOnly .ttf files are supported currently"
#define FRDR_E_MSG_SUBTABLE "Error while reading and parsing .ttf subtables"
#define FRDR_E_MSG_TABLE "Error while reading and parsing .ttf tables"
#define FRDR_E_MSG_RTABLE "Could not register %s table"

#define FRDR_E_MSG_TTFSTRUCT "Initialization of font structure failed"
#define FRDR_E_MSG_SUBSTRUCT "Initialization of subtables structures failed"

#define FRDR_E_MSG_ALLOC "Failed to allocate %s"
#define FRDR_E_MSG_FETCH "Could not fetch %s"

#define FRDR_E_MSG_FBUFF "Error while buffering file"
#define FRDR_E_MSG_FOPEN "Failed to open font file `%s`"
#define FRDR_E_MSG_FREAD "Failed to read font file `%s`"
#define FRDR_E_MSG_FSIZE "Failed to determine file size"

#define FRDR_E_MSG_MTABLE "Invalid font offset table values, file may be corrupted"
#define FRDR_E_MSG_SFNT "Invalid sfnt version, font isn't in a TTF format or file may be corrupted\nFound sfnt version %d, expected %d"

#define FRDR_E_MSG_TAG "All mandatory tables were not found properly, file may be incompatible or corrupted"
#define FRDR_E_MSG_ENTRIES "Failed to correctly determine all table entries"

#define FRDR_E_MSG_OFFSET "Could not get %s offset"

#define FRDR_E_MSG_FCMAP "Invalid CMAP format, file may be incompatible\nFound %d expected %d"

void	register_frdr_errors(void);

#endif// FRDR_XCERRCAL_H
