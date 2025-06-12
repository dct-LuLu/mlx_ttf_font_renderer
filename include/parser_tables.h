/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tables.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 10:14:51 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/12 18:44:54 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_TABLES_H
# define PARSER_TABLES_H

# include "file_utils.h"

# define REPEAT_FLAG 0x08

# define X_SHORT 0x02
# define X_IS_POSITIVE 0x10
# define X_IS_SAME 0x10

# define Y_SHORT 0x04
# define Y_IS_POSITIVE 0x20
# define Y_IS_SAME 0x20

# define SKIP_INSTRUCTION_BYTES 2

# define ARG1_ARG2_ARE_WORDS 0x0001       // arguments are 16bit instead of 8bit
# define ARGS_ARE_XY_VALUES 0x0002        // arguments are offsets not point
# define ROUND_XY_TO_GRID 0x0004          // round offsets to grid
# define HAS_SCALE 0x0008                 // single scale value present
# define MORE_COMPONENTS 0x0020           // more components follow
# define HAS_XY_SCALE 0x0040              // separate x/y scales present
# define HAS_2X2_MATRIX 0x0080            // full 2x2 transformation matrix
# define HAS_INSTRUCTIONS 0x0100          // instructions follow components
# define USE_METRICS 0x0200               // use this component's metrics
# define OVERLAP_COMPOUND 0x0400          // components overlap
# define SCALED_COMPONENT_OFFSET 0x0800   // scale offset values
# define UNSCALED_COMPONENT_OFFSET 0x1000 // don't scale offset values

typedef union u_delta
{
	struct
	{
		int8_t delta8;
		int8_t endiant;
	};
	int16_t					delta16;
}							t_delta;



# pragma pack(push, 1)

typedef struct s_head_table
{
	uint16_t				units_per_em;
	int16_t					x_min;
	int16_t					y_min;
	int16_t					x_max;
	int16_t					y_max;
	uint16_t				index_to_loc_format;
}							t_head_table;

typedef struct s_maxp_table
{
	uint16_t				num_glyphs;
}							t_maxp_table;

typedef struct s_hhea_table
{
	uint16_t				num_lhmtx;
}							t_hhea_table;

typedef struct s_cmap_table
{
	uint16_t				format;
	uint16_t				seg_count;
	uint16_t				seg_count_x2;
	uint16_t				*end_code;
	uint16_t				*start_code;
	int16_t					*id_delta;
	uint16_t				*id_range_offset;
	uint16_t				glyph_id_count;
	uint16_t				*glyph_id_array;
	size_t					pos_id_range_offset;
}							t_cmap_table;

typedef struct s_loca_table
{
	void					*offsets;
	uint16_t				format;
}							t_loca_table;

typedef struct s_lhmtx
{
	uint16_t				advance_width;
	int16_t					lsb;
}							t_lhmtx;

typedef struct s_hmtx_table
{
	uint16_t				num_lhmtx;
	t_lhmtx					*lhmtx;
	uint16_t				num_lsbs;
	int16_t					*lsbs;
}							t_hmtx_table;

typedef struct s_glyf_header
{
	int16_t					number_of_contours;
	int16_t					x_min;
	int16_t					y_min;
	int16_t					x_max;
	int16_t					y_max;
}							t_glyf_header;

typedef struct s_glyf_component
{
	uint16_t				flags;
	uint16_t				glyph_index;
	int16_t					arg1;
	int16_t					arg2;
	float					transform[4]; // xscale, scale01, scale10, yscale
	struct s_glyf_component	*next;
}							t_glyf_component;

typedef struct s_glyf_table
{
	t_glyf_header			*header;
	uint16_t				*end_pts;
	uint8_t					*flags;
	int16_t					*x_coordinates;
	int16_t					*y_coordinates;
	uint16_t				point_count;
	t_glyf_component		*component; // for composite glyphs
	uint16_t				instruction_length;
	uint8_t					*instructions;
}							t_glyf_table;

# pragma pack(pop)

#endif // PARSER_TABLES_H
