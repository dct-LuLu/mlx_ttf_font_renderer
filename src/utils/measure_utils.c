/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   measure_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 12:28:17 by jaubry--          #+#    #+#             */
/*   Updated: 2025/11/05 13:11:47 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

/*
	Measure char length doesnt take into account '\m'
*/
int	measure_char_width(char c, t_text *text)
{
	const size_t	glyf_idx = get_glyph_index(text->font, c);
	uint16_t		width;

	if ((glyf_idx >= text->font->maxp->num_glyphs)
			|| !text->font->hmtx || (c == '\n'))
		return (0);
	if (glyf_idx < text->font->hmtx->num_lhmtx)
		width = text->font->hmtx->lhmtx[glyf_idx].advance_width;
	else
		width = text->font->hmtx->lhmtx[text->font->hmtx->num_lhmtx - 1].advance_width;
	return (scale_x(text, 0, width));
}

/*
	Measure str length as if it was a single line doesnt take into account '\m'
*/
int	measure_str_width(char *str, t_text *text)
{
	int		total_width;
	size_t	i;

	i = 0;
	total_width = 0;
	while (str[i])
	{
		total_width += measure_char_width(str[i], text);
		i++;
	}
	return (total_width);
}

int	measure_text_line_width(char *str, t_text *text)
{
	int		line_width;
	size_t	i;

	i = 0;
	line_width = 0;
	while (str[i])
	{
		if (str[i] == '\n')
			break ;
		line_width += measure_char_width(str[i], text);
		i++;
	}
	return (line_width);
}

/*
	Measure total width (max width) taking into account '\n'
*/
int	measure_text_total_width(char *str, t_text *text)
{
	int		max_width;
	int		width;
	char	**strs;
	size_t	i;

	i = 0;
	max_width = 0;
	strs = ft_split((char *)str, '\n');
	if (!strs)
		return (0);//error
	while (strs[i])
	{
		width = measure_str_width(strs[i], text);
		if (width > max_width)
			max_width = width;
		i++;
	}
	free_strr(strs);
	return (max_width);
}
