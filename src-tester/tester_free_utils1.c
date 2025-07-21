/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 10:02:56 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/11 14:04:32 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"
#include <stdlib.h>

void	free_table_cmap(t_ttf_font *font);
void	free_table_hmtx(t_ttf_font *font);
void	free_table_loca(t_ttf_font *font);
void	free_table_glyfs(t_ttf_font *font);

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
