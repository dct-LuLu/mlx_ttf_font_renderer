/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_table_glyf_composite.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 13:19:11 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/11 13:32:23 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "parser_font_ttf.h"

void	debug_glyf_component(t_glyf_component comp)
{
	printf("\t\tComponent:\n\t\t{\n");
	printf("\t\t\tglyph_index: %u\n", comp.glyph_index);
	printf("\t\t\tflags: 0x%04X\n", comp.flags);
	printf("\t\t\targ1: %d\n", comp.arg1);
	printf("\t\t\targ2: %d\n", comp.arg2);
	if (comp.flags & HAS_SCALE)
		printf("\t\t\tscale: %f\n", comp.transform[0]);
	else if (comp.flags & HAS_XY_SCALE)
		printf("\t\t\txscale: %f, yscale: %f\n", comp.transform[0],
			comp.transform[3]);
	else if (comp.flags & HAS_2X2_MATRIX)
		printf("\t\t\ttransform: [%f %f; %f %f]\n", comp.transform[0],
			comp.transform[1], comp.transform[2], comp.transform[3]);
	printf("\t\t}\n");
}

static float	read_f2dot14(t_buffer *buf)
{
	int16_t	value;

	read_bytes(buf, &value, 2);
	value = be16toh(value);
	return ((float)value / 16384.0f);
}

static void	continue_parse_component(t_glyf_component *comp, t_buffer *buf)
{
	if (comp->flags & HAS_SCALE)
	{
		comp->transform[0] = read_f2dot14(buf);
		comp->transform[3] = comp->transform[0];
	}
	else if (comp->flags & HAS_XY_SCALE)
	{
		comp->transform[0] = read_f2dot14(buf);
		comp->transform[3] = read_f2dot14(buf);
	}
	else if (comp->flags & HAS_2X2_MATRIX)
	{
		comp->transform[0] = read_f2dot14(buf);
		comp->transform[1] = read_f2dot14(buf);
		comp->transform[2] = read_f2dot14(buf);
		comp->transform[3] = read_f2dot14(buf);
	}
}

static int	parse_component(t_glyf_component **comp, t_buffer *buf)
{
	int16_t	args16[2];
	int8_t	args8[2];

	*comp = ft_calloc(1, sizeof(t_glyf_component));
	if (!*comp)
		return (rerror(errno, "t_glyf_component"));
	(*comp)->transform[0] = 1.0f;
	(*comp)->transform[3] = 1.0f;
	read_bytes(buf, &(*comp)->flags, 2);
	(*comp)->flags = be16toh((*comp)->flags);
	read_bytes(buf, &(*comp)->glyph_index, 2);
	(*comp)->glyph_index = be16toh((*comp)->glyph_index);
	if ((*comp)->flags & ARG1_ARG2_ARE_WORDS)
	{
		read_bytes(buf, args16, 4);
		(*comp)->arg1 = be16toh(args16[0]);
		(*comp)->arg2 = be16toh(args16[1]);
	}
	else
	{
		read_bytes(buf, args8, 2);
		(*comp)->arg1 = args8[0];
		(*comp)->arg2 = args8[1];
	}
	return (continue_parse_component(*comp, buf), 0);
}

int	parse_composite_glyf(t_glyf_table *glyf, t_buffer *buf)
{
	t_glyf_component	**next_ptr;
	uint16_t			flags;

	next_ptr = &glyf->component;
	flags = MORE_COMPONENTS;
	while (flags & MORE_COMPONENTS)
	{
		if (parse_component(next_ptr, buf))
			return (1);
		flags = (*next_ptr)->flags;
		next_ptr = &((*next_ptr)->next);
	}
	if (flags & HAS_INSTRUCTIONS)
	{
		read_bytes(buf, &glyf->instruction_length, 2);
		glyf->instruction_length = be16toh(glyf->instruction_length);
		if (glyf->instruction_length > 0)
		{
			glyf->instructions = ft_calloc(glyf->instruction_length, 1);
			if (!glyf->instructions)
				return (rerror(errno, "glyf->instructions"));
			read_bytes(buf, glyf->instructions, glyf->instruction_length);
		}
	}
	return (0);
}
