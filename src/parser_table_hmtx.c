/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_table_hmtx.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 16:02:23 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/28 02:24:41 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "file_utils.h"
#include "libft.h"
#include "parser_font_ttf.h"

static void	debug_table_hmtx(t_hmtx_table hmtx)
{
	size_t	i;

	printf("HMTX Table:\n{\n");
	printf("\tnum_lhmtx: %u\n", hmtx.num_lhmtx);
	printf("\tFirst few metrics:\n\t{\n");
	i = 0;
	while ((i < DEBUG_NUM) && (i < hmtx.num_lhmtx))
	{
		printf("\t\t[%zu] advance_width: %u,\tlsb: %d\n", i,
			hmtx.lhmtx[i].advance_width, hmtx.lhmtx[i].lsb);
		i++;
	}
	printf("\t}\n");
	if (hmtx.lsbs)
	{
		printf("\t First few lsbs:\n");
		i = 0;
		while ((i < DEBUG_NUM) && (i < hmtx.num_lsbs))
		{
			printf("\t\t[%zu] %u\n", i, hmtx.lsbs[i]);
			i++;
		}
	}
	printf("}\n\n");
}

static void	endian_swap_table_hmtx(t_hmtx_table *hmtx)
{
	size_t	i;

	i = 0;
	while (i < hmtx->num_lhmtx)
	{
		hmtx->lhmtx[i].advance_width = be16toh(hmtx->lhmtx[i].advance_width);
		hmtx->lhmtx[i].lsb = be16toh(hmtx->lhmtx[i].lsb);
		i++;
	}
	if (hmtx->lsbs)
	{
		i = 0;
		while (i < hmtx->num_lsbs)
		{
			hmtx->lsbs[i] = be16toh(hmtx->lsbs[i]);
			i++;
		}
	}
}

int	parse_table_hmtx(t_ttf_font *font, t_buffer *buf)
{
	const ssize_t	hmtx_offset = get_table_offset(font, HMTX_TAG);
	t_hmtx_table	*hmtx;

	if (hmtx_offset == -1)
		return (error(ERR_GET_OFFSET, ": hmtx"));
	hmtx = ft_calloc(sizeof(t_hmtx_table), 1);
	if (!hmtx)
		return (error(errno, "t_hmtx_table"));
	hmtx->num_lhmtx = font->hhea->num_lhmtx;
	hmtx->lhmtx = ft_calloc(sizeof(t_lhmtx), hmtx->num_lhmtx);
	if (!hmtx->lhmtx)
		return (error(errno, "hmtx->h_metrics"));
	buf->pos = hmtx_offset;
	read_bytes(buf, hmtx->lhmtx, hmtx->num_lhmtx * sizeof(t_lhmtx));
	hmtx->num_lsbs = font->maxp->num_glyphs - hmtx->num_lhmtx;
	if (hmtx->num_lsbs > 0)
	{
		hmtx->lsbs = ft_calloc(sizeof(int16_t), hmtx->num_lsbs);
		if (!hmtx->lsbs)
			return (error(errno, "hmtx->left_side_bearing"));
		read_bytes(buf, hmtx->lsbs, hmtx->num_lsbs * sizeof(int16_t));
	}
	endian_swap_table_hmtx(hmtx);
	if (DEBUG)
		debug_table_hmtx(*hmtx);
	font->hmtx = hmtx;
	return (0);
}
