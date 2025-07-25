/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 12:09:17 by jaubry--          #+#    #+#             */
/*   Updated: 2025/07/17 18:08:47 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MLX_UTILS_H
# define MLX_UTILS_H
# include "libft.h"
# include "vec2.h"
# include <X11/X.h>
# include <X11/keysym.h>
# include <mlx.h>

# ifndef WIDTH
#  define WIDTH 2000
# endif
# ifndef HEIGHT
#  define HEIGHT 2000
# endif

# define BACKGROUND 0x0F0000000

typedef struct s_img
{
	void			*img;
	char			*addr;
	int				byte_depth;
	int				line_len;
	int				endian;
	int				width;
	int				height;
}					t_img;

typedef struct s_mlx
{
	void			*mlx;
	void			*win;
	t_img			img;
	t_vec2			origin;
	t_vec2			size;
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

t_img	init_img(void *mlx, int width, int height);
void	ft_mlx_pixel_put(t_img *data, t_vec2 pos, int color);
void	ft_mlx_line_put(t_img *data, t_vec2 a, t_vec2 b, int color);
void	ft_mlx_batch_put(t_img *data, t_vec2 pos, t_vec2 size, int color);
void	ft_mlx_draw_quadratic_curve(t_img *img, t_vec2 *pts, int color);
void	ft_mlx_horizontal_line(t_img *img, int xpt[2], int y, int color);
void	ft_mlx_circle_put(t_img *img, t_vec2 center, int radius, int color);

int		kill_mlx(t_mlx *mlx);
void	kill_img(void *mlx, t_img *img);

#endif // MLX_UTILS_H
