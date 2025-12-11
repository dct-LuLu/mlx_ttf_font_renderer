/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   measure_height_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 20:53:59 by jaubry--          #+#    #+#             */
/*   Updated: 2025/12/04 21:47:18 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

int	measure_line_height(t_text *text)
{
	return (-scale_y(text, 0, (text->font->hhea->ascent + abs(text->font->hhea->descent))));
}
