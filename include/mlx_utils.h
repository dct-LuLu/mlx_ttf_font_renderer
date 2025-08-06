/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 12:09:17 by jaubry--          #+#    #+#             */
/*   Updated: 2025/08/06 09:10:03 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MLX_UTILS_H
# define MLX_UTILS_H
# include "libft.h"
# include "vectors.h"
# include <X11/X.h>
# include <X11/keysym.h>
# include <mlx.h>
# include <mlx_int.h>


# ifndef WIDTH
#  define WIDTH 2000
# endif
# ifndef HEIGHT
#  define HEIGHT 2000
# endif

# define BACKGROUND 0x0F0000000

typedef struct s_img_data
{
	t_img			*img;
	int				*addr;
	int				byte_depth;
	int				line_len;
	int				endian;
	int				width;
	int				height;
}					t_img_data;

typedef struct s_mlx
{
	t_xvar			*mlx;
	t_win_list		*win;
	t_img_data		img;
	t_vec2i			origin;
	t_vec2i			size;
}					t_mlx;

typedef struct s_line
{
	int				dx;
	int				dy;
	int				sx;
	int				sy;
	int				err;
	int				e2;
}					t_line;

t_img_data	init_img_data(t_xvar *mlx, int width, int height);
void		ft_mlx_pixel_put(t_img_data *data, t_vec2i pos, int color);
void		ft_mlx_line_put(t_img_data *data, t_vec2i a, t_vec2i b, int color);
void		ft_mlx_batch_put(t_img_data *data, t_vec2i pos, t_vec2i size, int color);
void		ft_mlx_draw_quadratic_curve(t_img_data *img, t_vec2i *pts, int color);
void		ft_mlx_horizontal_line(t_img_data *img, int xpt[2], int y, int color);
void		ft_mlx_circle_put(t_img_data *img, t_vec2i center, int radius, int color);

int		kill_mlx(t_mlx *mlx);
void	kill_img(t_xvar *mlx, t_img_data *img);

#endif // MLX_UTILS_H
