/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_fps.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 01:52:47 by jaubry--          #+#    #+#             */
/*   Updated: 2025/11/04 18:28:52 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

static size_t	ft_itoalen(long int nb)
{
	size_t	len;

	if (nb == 0)
		return (1);
	len = 0;
	if (nb < 0)
	{
		nb *= -1;
		len++;
	}
	while (nb > 0)
	{
		nb /= 10;
		len++;
	}
	return (len);
}

static void	ft_itoal(int n, unsigned char *str)
{
	long int	num;
	size_t		i;
	size_t		len;

	i = 0;
	num = (long int)n;
	len = ft_itoalen(num);
	str[len] = '\0';
	str[0] = '0';
	if (num < 0)
	{
		str[0] = '-';
		num *= -1;
	}
	while (num > 0)
	{
		str[len - i - 1] = '0' + (num % 10);
		num /= 10;
		i++;
	}
}
typedef struct s_test_env
{
	t_ttf_font	*font;
	t_text		*fps;
	t_text		*test;
	t_mlx		*mlx_data;
}	t_test_env;

void	update_fps(t_mlx *mlx_data, t_text *fps)
{
	ft_itoal((int)mlx_data->fps, fps->content);
}

void	init_fps(t_test_env *env)
{
	const int	size = 3;

	env->fps = ft_calloc(1, sizeof(t_text));
	env->fps->pos = vec2i(0, 0);
	env->fps->fg = (t_rgba_int){.rgba=YELLOW};
	env->fps->font_size = size;
	env->fps->img = &(env->mlx_data->img);
	env->fps->font = env->font;
	env->fps->subpixel = true;
}

