/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_table_glyf_composite.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 13:19:11 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/22 14:31:08 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_font_ttf.h"
#include "libft.h"

static float	read_f2dot14(t_buffer *buf)
{
	int16_t	value;

	read_bytes(buf, &value, 2);
	value = be16toh(value);
	return ((float)value / 16384.0f);
}

static int	parse_component(t_glyf_component **component, t_buffer *buf)
{
	t_glyf_component	*comp;

	*component = ft_calloc(1, sizeof(t_glyf_component));
	if (!*component)
		return (error(errno, "t_glyf_component"));
	comp = *component;
	// init transform matrix to identify;
	comp->transform[0] = 1.0f;
	comp->transform[3] = 1.0f;

	// read flags and idx
	read_bytes(buf, &comp->flags, 2);
	comp->flags = be16toh(comp->flags);

	read_bytes(buf, &comp->glyph_index, 2);
	comp->glyph_index = be16toh(comp->glyph_index);

	// Read arguments based on flags
	if (comp->flags & ARG1_ARG2_ARE_WORDS)
	{
		int16_t	args[2];
		read_bytes(buf, args, 4);
		comp->arg1 = be16toh(args[0]);
		comp->arg2 = be16toh(args[1]);
	}
	else
	{
		int8_t	args[2];
		read_bytes(buf, args, 2);
		comp->arg1 = args[0];
		comp->arg2 = args[1];
	}

	// read transformation data if present
	if (comp->flags & WE_HAVE_SCALE)
	{
		comp->transform[0] = read_f2dot14(buf);
		comp->transform[3] = comp->transform[0];
	}
	else if (comp->flags & XY_SCALE)
	{
		comp->transform[0] = read_f2dot14(buf);
		comp->transform[3] = read_f2dot14(buf);
	}
	else if (comp->flags & WE_HAVE_2X2)
	{
		comp->transform[0] = read_f2dot14(buf);
		comp->transform[1] = read_f2dot14(buf);
		comp->transform[2] = read_f2dot14(buf);
		comp->transform[3] = read_f2dot14(buf);
	}
	return (0);
}

int	parse_composite_glyf(t_glyf_table *glyf, t_buffer *buf)
{
	t_glyf_componnet	**next_ptr = &glyf->components;
	uint16_t			flags = 0;

	while (flags & MORE_COMPONENTS)
	{
		if (parse_component(next_ptr, buf))
			return (1);
		flags = (*next_ptr)->flags;
		next_ptr = &((*next_ptr)->next);
	}

	if (flags & WE_HAVE_INSTRUCTIONS)
	{
		read_bytes(buf, &glyf->instruction_length, 2);
		glyf->instruction_length = be16toh(glyf->instruction_length);
		if (glyf->instruction_length > 0)
		{
			glyf->instructions = ft_calloc(glyf->instruction_length, 1);
			if (!glyf->instructions)
				return (error(errno, "glyf->instructions"));
			read_bytes(buf, glyf->instructions, glyf->instruction_length);
		}
	}
	return (0);
}
