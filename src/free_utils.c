/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 10:02:56 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/15 10:29:10 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "parser_font_ttf.h"

void	free_ttf(t_ttf_font *font)
{
	size_t	i;

	if (font->ot)
		free(font->ot);
	if (font->entries)
	{
		i = 0;
		while (i < MANDATORY_TAG_NB)
		{
			if (font->entries[i])
				free(font->entries[i]);
			i++;
		}
		free(font->entries);
	}
	free(font);
}
