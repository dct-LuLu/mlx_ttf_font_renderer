/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tables.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 10:14:51 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/21 20:27:18 by jaubry--         ###   ########lyon.fr   */
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

# pragma pack(push, 1)

typedef struct s_head_table
{
	uint16_t	units_per_em;			// Font units per em
	int16_t		x_min;					// Minimum x coordinate across all glyphs
	int16_t		y_min;					// Minimum y coordinate across all glyphs
	int16_t		x_max;					// Maximum x coordinate across all glyphs
	int16_t		y_max;					// Maximum y coordinate across all glyphs
	uint16_t 	index_to_loc_format;	// Format of loca table (0=short, 1=long)
}				t_head_table;



typedef struct s_maxp_table
{
    uint16_t	num_glyphs;				// Number of glyphs in the font
} 				t_maxp_table;


typedef struct s_hhea_table
{
	uint16_t	num_lhmtx;
}				t_hhea_table;


typedef struct s_cmap_table
{
	uint16_t	format;					// Format number (4)
	uint16_t	seg_count;
	uint16_t	seg_count_x2;			// 2 × segCount
	uint16_t	*end_code;				// End characterCode for each segment
	uint16_t	*start_code;			// Start characterCode for each segment
	int16_t		*id_delta;				// Delta for all character codes in segment
	uint16_t	*id_range_offset;		// Offset in bytes to glyph indexArray
	uint16_t	glyph_id_count;
	uint16_t	*glyph_id_array;		// Glyph index array
	size_t		pos_id_range_offset;
}				t_cmap_table;



typedef struct s_loca_table
{
	void		*offsets;				// Array of offsets (either uint16_t or uint32_t)
	uint16_t	format;					// 0=short (16-bit), 1=long (32-bit)
}				t_loca_table;



typedef struct s_lhmtx
{
	uint16_t	advance_width;
	int16_t		lsb;
}				t_lhmtx;

typedef struct s_hmtx_table
{
	uint16_t	num_lhmtx;// Number of hMetrics entries
	t_lhmtx		*lhmtx;
	uint16_t	num_lsbs;
	int16_t		*lsbs;// Additional LSBs for glyphs
}				t_hmtx_table;



typedef struct s_glyf_header
{
	int16_t	number_of_contours;			// If >= 0, simple glyph; if < 0, composite glyph
	int16_t	x_min;						// Minimum x coordinate
	int16_t	y_min;						// Minimum y coordinate
	int16_t	x_max;						// Maximum x coordinate
	int16_t	y_max;						// Maximum y coordinate
}			t_glyf_header;

typedef struct s_glyf_table
{
	t_glyf_header	*header;
	uint16_t		*end_pts;			// Array of contour end points
	uint8_t			*flags;				// Array of flags
	int16_t			*x_coordinates;		// Array of x coordinates
	int16_t			*y_coordinates;		// Array of y coordinates
	uint16_t		point_count;		// Total number of points
}					t_glyf_table;

# pragma pack(pop)


#endif //PARSER_TABLES_H
