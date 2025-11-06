/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_fps.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 01:52:47 by jaubry--          #+#    #+#             */
/*   Updated: 2025/11/04 17:03:30 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"
#include "tester.h"

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

void	update_fps(t_env *env)
{
	ft_itoal((int)env->mlx->fps, env->fps->content);
}

void	init_fps(t_env *env, t_ttf_font *font)
{
	t_text	*fps;

	fps = ft_calloc(1, sizeof(t_text));
	fps->_text_pos = vec2i(0, 0);
	fps->fg = (t_rgba_int){.rgba=YELLOW};
	fps->font_size = 5;
	fps->img = &(env->mlx->img);
	fps->font = font;
	fps->subpixel = true;
	env->fps = fps;
}

