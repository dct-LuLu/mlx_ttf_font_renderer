/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_fps.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 01:52:47 by jaubry--          #+#    #+#             */
/*   Updated: 2025/07/16 01:52:51 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

ssize_t	get_current_time(void);

static int	get_fps(void)
{
	static ssize_t	t1 = -1;
	static ssize_t	t2 = -1;

	t1 = t2;
	t2 = get_current_time();
	return (1000 / (t2 - t1));
}

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

static void	ft_itoal(int n, char *str)
{
	long int	num;
	size_t		i;
	size_t		len;

	i = 0;
	num = (long int)n;
	len = ft_itoalen(num);
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

void	show_fps(t_env *env)
{
	static char		fps[5];
	const t_vec2	pos = new_vec2(200, 200);

	ft_itoal(get_fps(), fps);
	draw_string(env, fps, pos, YELLOW);
}
