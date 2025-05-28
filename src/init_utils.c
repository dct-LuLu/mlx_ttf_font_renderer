/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 10:03:36 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/28 02:23:04 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "parser_font_ttf.h"

static int	init_subtables(t_ttf_font *font)
{
	size_t	i;

	font->ot = ft_calloc(sizeof(t_offset_subtable), 1);
	if (!font->ot)
		return (error(errno, "t_offset_subtable"));
	font->entries = ft_calloc(sizeof(t_subtable_entry *), MANDATORY_TAG_NB);
	if (!font->entries)
		return (error(errno, "*t_subtable_entry"));
	i = 0;
	while (i < MANDATORY_TAG_NB)
	{
		font->entries[i] = ft_calloc(sizeof(t_subtable_entry), 1);
		if (!font->entries[i])
			return (error(errno, "t_subtable_entry[%d]", i));
		i++;
	}
	return (0);
}

int	init_ttf_struct(t_ttf_font **font)
{
	int	ret;

	ret = 0;
	*font = ft_calloc(sizeof(t_ttf_font), 1);
	if (!*font)
		return (error(errno, "t_ttf_font"));
	ret = init_subtables(*font);
	return (ret);
}
