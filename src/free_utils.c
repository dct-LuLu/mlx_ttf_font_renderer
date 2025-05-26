/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 10:02:56 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/26 12:51:02 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "font_renderer.h"

static void	free_subtables(t_ttf_font *font)
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
}

static void	free_table_cmap(t_ttf_font *font)
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

static void	free_table_hmtx(t_ttf_font *font)
{
	if (font->hmtx->lhmtx)
		free(font->hmtx->lhmtx);
	if (font->hmtx->lsbs)
		free(font->hmtx->lsbs);
	free(font->hmtx);
}

static void	free_table_loca(t_ttf_font *font)
{
	if (font->loca->offsets)
		free(font->loca->offsets);
	free(font->loca);
}

static void	free_composite_glyf(t_glyf_component *comp)
{
	if (comp)
	{
		free_composite_glyf(comp->next);
		free(comp);
	}
}

static void	free_table_glyfs(t_ttf_font *font)
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
			if (font->glyfs[i]->end_pts)
				free(font->glyfs[i]->end_pts);
			if (font->glyfs[i]->flags)
				free(font->glyfs[i]->flags);
			if (font->glyfs[i]->x_coordinates)
				free(font->glyfs[i]->x_coordinates);
			if (font->glyfs[i]->y_coordinates)
				free(font->glyfs[i]->y_coordinates);
			if (font->glyfs[i]->instructions)
				free(font->glyfs[i]->instructions);
			free(font->glyfs[i]);
		}
		i++;
	}
	free(font->glyfs);
}

static void	free_tables(t_ttf_font *font)
{
	if (font->head)
		free(font->head);
	if (font->cmap)
		free_table_cmap(font);
	if (font->hhea)
		free(font->hhea);
	if (font->hmtx)
		free_table_hmtx(font);		
	if (font->loca)
		free_table_loca(font);
	if (font->glyfs)
		free_table_glyfs(font);
	if (font->maxp)
		free(font->maxp);
}

static void	free_buf(t_buffer *buf)
{
	if (buf)
	{
		free(buf->data);
		free(buf);
	}
}

static void	free_ttf(t_ttf_font *font)
{
	if (font)
	{
		free_subtables(font);
		free_tables(font);
		free_buf(font->buf);
		free(font);
	}
}

void	free_env(t_env *env)
{
	if (env)
	{
		free_ttf(env->font);
		free(env);
	}
}
