/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:03:02 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/11 14:03:55 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"
#include <stdlib.h>

void	free_table_cmap(t_ttf_font *font)
{
	if (font->cmap->glyph_id_array)
		free(font->cmap->glyph_id_array);
	if (font->cmap->end_code)
		free(font->cmap->end_code);
	if (font->cmap->start_code)
		free(font->cmap->start_code);
	if (font->cmap->id_delta)
		free(font->cmap->id_delta);
	if (font->cmap->id_range_offset)
		free(font->cmap->id_range_offset);
	free(font->cmap);
}

void	free_table_hmtx(t_ttf_font *font)
{
	if (font->hmtx->lhmtx)
		free(font->hmtx->lhmtx);
	if (font->hmtx->lsbs)
		free(font->hmtx->lsbs);
	free(font->hmtx);
}

void	free_table_loca(t_ttf_font *font)
{
	if (font->loca->offsets)
		free(font->loca->offsets);
	free(font->loca);
}
