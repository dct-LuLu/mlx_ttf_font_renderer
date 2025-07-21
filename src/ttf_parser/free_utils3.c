/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:03:14 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/11 14:04:40 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"
#include <stdlib.h>

static void	free_composite_glyf(t_glyf_component *comp)
{
	if (comp)
	{
		free_composite_glyf(comp->next);
		free(comp);
	}
}

static void	free_glyf_attr(t_glyf_table *glyf)
{
	if (glyf->end_pts)
		free(glyf->end_pts);
	if (glyf->flags)
		free(glyf->flags);
	if (glyf->x_coordinates)
		free(glyf->x_coordinates);
	if (glyf->y_coordinates)
		free(glyf->y_coordinates);
	if (glyf->instructions)
		free(glyf->instructions);
}

void	free_table_glyfs(t_ttf_font *font)
{
	size_t	i;

	i = 0;
	while (i < font->maxp->num_glyphs)
	{
		if (font->glyfs[i])
		{
			if (font->glyfs[i]->header)
			{
				if (font->glyfs[i]->header->number_of_contours == -1)
					free_composite_glyf(font->glyfs[i]->component);
				free(font->glyfs[i]->header);
			}
			free_glyf_attr(font->glyfs[i]);
			free(font->glyfs[i]);
		}
		i++;
	}
	free(font->glyfs);
}
