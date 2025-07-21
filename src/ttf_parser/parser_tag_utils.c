/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tag_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 14:53:35 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/28 02:25:00 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "parser_font_ttf.h"

enum e_entry_tag	get_entry_tag_type(t_subtable_entry entry)
{
	if (ft_strncmp(entry.tag, "cmap", 4) == 0)
		return (CMAP_TAG);
	else if (ft_strncmp(entry.tag, "glyf", 4) == 0)
		return (GLYF_TAG);
	else if (ft_strncmp(entry.tag, "loca", 4) == 0)
		return (LOCA_TAG);
	else if (ft_strncmp(entry.tag, "hhea", 4) == 0)
		return (HHEA_TAG);
	else if (ft_strncmp(entry.tag, "hmtx", 4) == 0)
		return (HMTX_TAG);
	else if (ft_strncmp(entry.tag, "head", 4) == 0)
		return (HEAD_TAG);
	else if (ft_strncmp(entry.tag, "maxp", 4) == 0)
		return (MAXP_TAG);
	return (NON_MANDATORY_TAG);
}

ssize_t	get_table_offset(t_ttf_font *font, enum e_entry_tag tag)
{
	size_t	i;

	i = 0;
	while (i < MANDATORY_TAG_NB)
	{
		if (get_entry_tag_type(*(font->entries[i])) == tag)
			return (font->entries[i]->offset);
		i++;
	}
	return (-1);
}
