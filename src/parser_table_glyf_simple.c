/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_table_glyf_simple.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 20:29:39 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/28 02:23:52 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "error_handler.h"
#include "file_utils.h"
#include "libft.h"
#include "parser_font_ttf.h"

static int	parse_glyf_flags(t_glyf_table *glyf, t_buffer *buf)
{
	size_t	i;
	uint8_t	flag;
	uint8_t	repeat_count;

	glyf->flags = ft_calloc(glyf->point_count, sizeof(uint8_t));
	if (!glyf->flags)
		return (1);
	i = 0;
	while (i < glyf->point_count)
	{
		repeat_count = 1;
		read_bytes(buf, &flag, 1);
		glyf->flags[i] = flag;
		if (flag & REPEAT_FLAG)
		{
			read_bytes(buf, &repeat_count, 1);
			while ((repeat_count > 0) && ((i + 1) < glyf->point_count))
			{
				glyf->flags[++i] = flag;
				repeat_count--;
			}
		}
		i++;
	}
	return (0);
}

static void	parse_glyf_x_coordinates(t_glyf_table *glyf, t_buffer *buf)
{
	int16_t	delta16;
	uint8_t	delta8;
	int16_t	x;
	size_t	i;

	x = 0;
	i = 0;
	while (i < glyf->point_count)
	{
		if (glyf->flags[i] & X_SHORT)
		{
			read_bytes(buf, &delta8, 1);
			if (glyf->flags[i] & X_IS_POSITIVE)
				x += delta8;
			else
				x -= delta8;
		}
		else if (!(glyf->flags[i] & X_IS_SAME))
		{
			read_bytes(buf, &delta16, 2);
			delta16 = be16toh(delta16);
			x += delta16;
		}
		glyf->x_coordinates[i] = x;
		i++;
	}
}

static void	parse_glyf_y_coordinates(t_glyf_table *glyf, t_buffer *buf)
{
	int16_t	delta16;
	uint8_t	delta8;
	int16_t	y;
	size_t	i;

	y = 0;
	i = 0;
	while (i < glyf->point_count)
	{
		if (glyf->flags[i] & Y_SHORT)
		{
			read_bytes(buf, &delta8, 1);
			if (glyf->flags[i] & Y_IS_POSITIVE)
				y += delta8;
			else
				y -= delta8;
		}
		else if (!(glyf->flags[i] & Y_IS_SAME))
		{
			read_bytes(buf, &delta16, 2);
			delta16 = be16toh(delta16);
			y += delta16;
		}
		glyf->y_coordinates[i] = y;
		i++;
	}
}

static int	parse_glyf_coordinates(t_glyf_table *glyf, t_buffer *buf)
{
	glyf->x_coordinates = ft_calloc(glyf->point_count, sizeof(int16_t));
	if (!glyf->x_coordinates)
		return (1);
	glyf->y_coordinates = ft_calloc(glyf->point_count, sizeof(int16_t));
	if (!glyf->y_coordinates)
		return (1);
	parse_glyf_x_coordinates(glyf, buf);
	parse_glyf_y_coordinates(glyf, buf);
	return (0);
}

int	parse_simple_glyf(t_glyf_table *glyf, t_buffer *buf)
{
	uint16_t	last_point;
	int16_t		i;

	glyf->end_pts = ft_calloc(glyf->header->number_of_contours,
			sizeof(uint16_t));
	if (!glyf->end_pts)
		return (1);
	read_bytes(buf, glyf->end_pts, glyf->header->number_of_contours * 2);
	i = 0;
	while (i < glyf->header->number_of_contours)
	{
		glyf->end_pts[i] = be16toh(glyf->end_pts[i]);
		i++;
	}
	last_point = glyf->end_pts[glyf->header->number_of_contours - 1];
	glyf->point_count = last_point + 1;
	read_bytes(buf, &glyf->instruction_length, SKIP_INSTRUCTION_BYTES);
	glyf->instruction_length = be16toh(glyf->instruction_length);
	if (glyf->instruction_length > 0) // vrm necessaire de check la taille ...?
	{
		glyf->instructions = ft_calloc(glyf->instruction_length, 1);
		if (!glyf->instructions)
			return (error(errno, "glyf instructions"));
		read_bytes(buf, glyf->instructions, glyf->instruction_length);
	}
	if (parse_glyf_flags(glyf, buf))
		return (1);
	return (parse_glyf_coordinates(glyf, buf));
}
