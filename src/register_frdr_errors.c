/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   register_frdr_errors.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 19:01:28 by jaubry--          #+#    #+#             */
/*   Updated: 2025/10/12 21:17:30 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "xcerrcal.h"
#include "frdr_xcerrcal.h"

void	register_frdr_errors(void)
{
	const char	*err_msgs[] =
	{
	FRDR_E_MSG_TTF,
	FRDR_E_MSG_EXT,
	FRDR_E_MSG_SUBTABLE,
	FRDR_E_MSG_TABLE,
	FRDR_E_MSG_RTABLE,
	FRDR_E_MSG_TTFSTRUCT,
	FRDR_E_MSG_SUBSTRUCT,
	FRDR_E_MSG_ALLOC,
	FRDR_E_MSG_FETCH,
	FRDR_E_MSG_FBUFF,
	FRDR_E_MSG_FOPEN,
	FRDR_E_MSG_FREAD,
	FRDR_E_MSG_FSIZE,
	FRDR_E_MSG_MTABLE,
	FRDR_E_MSG_SFNT,
	FRDR_E_MSG_TAG,
	FRDR_E_MSG_ENTRIES,
	FRDR_E_MSG_OFFSET,
	FRDR_E_MSG_FCMAP
	};
	bulk_register_error(FRDR_ERRS_NUM - 1, FRDR_ID, (const char **)err_msgs);
}
