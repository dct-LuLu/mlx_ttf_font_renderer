/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 10:03:36 by jaubry--          #+#    #+#             */
/*   Updated: 2025/10/12 19:51:45 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "parser_font_ttf.h"

static int	init_subtables(t_ttf_font *font)
{
	size_t	i;

	font->ot = ft_calloc(sizeof(t_offset_subtable), 1);
	if (!font->ot)
	{
		register_complex_err_msg(FRDR_E_MSG_ALLOC, "subtable offsets");
		return (error(pack_err(FRDR_ID, FRDR_E_ALLOC), FL, LN, FC));
	}
	font->entries = ft_calloc(sizeof(t_subtable_entry *), MANDATORY_TAG_NB);
	if (!font->entries)
	{
		register_complex_err_msg(FRDR_E_MSG_ALLOC, "array of subtable entries");
		return (error(pack_err(FRDR_ID, FRDR_E_ALLOC), FL, LN, FC));
	}
	i = 0;
	while (i < MANDATORY_TAG_NB)
	{
		font->entries[i] = ft_calloc(sizeof(t_subtable_entry), 1);
		if (!font->entries[i])
		{
			register_complex_err_msg(FRDR_E_MSG_ALLOC, "subtable entry");
			return (error(pack_err(FRDR_ID, FRDR_E_ALLOC), FL, LN, FC));
		}
		i++;
	}
	return (0);
}

int	init_ttf_struct(t_ttf_font **font)
{
	*font = ft_calloc(sizeof(t_ttf_font), 1);
	if (!*font)
		return (-1);
	if (init_subtables(*font) != 0)
		return (error(pack_err(FRDR_ID, FRDR_E_SUBSTRUCT), FL, LN, FC));
	return (0);
}
