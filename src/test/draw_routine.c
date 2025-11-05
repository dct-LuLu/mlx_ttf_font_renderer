/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_routine.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:39:37 by jaubry--          #+#    #+#             */
/*   Updated: 2025/11/05 13:14:03 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

void	update_fps(t_mlx *mlx_data,t_text *fps);
void	draw_text(t_text *text);

typedef struct s_test_env
{
	t_ttf_font	*font;
	t_text		*fps;
	t_text		*test;
	t_mlx		*mlx_data;
}	t_test_env;

int	draw_routine(t_test_env	*env)
{
	ft_mlx_batch_put(&env->mlx_data->img, env->mlx_data->origin, env->mlx_data->size, (t_rgb_int){.rgb=BACKGROUND});
	ft_mlx_line_put(&env->mlx_data->img, env->mlx_data->origin, env->mlx_data->size, (t_rgb_int){.rgb=0xFFFFFF});
	ft_mlx_line_put(&env->mlx_data->img, vec2i(env->mlx_data->size.x, env->mlx_data->origin.y), vec2i(env->mlx_data->origin.x, env->mlx_data->size.y), (t_rgb_int){.rgb=0xFFFFFF});
	update_fps(env->mlx_data, env->fps);
	if (env->test->rb.x - env->test->lt.x == 500)
		env->test->rb = env->test->lt;
	else
		env->test->rb = vec2i_add_scalar(env->test->rb, 1);
	ft_mlx_out_aarec_aput(&env->mlx_data->img, env->test->lt, env->test->rb, env->test->fg);
	draw_text(env->test);
	draw_text(env->fps);
	mlx_put_image_to_window(env->mlx_data->mlx, env->mlx_data->win, env->mlx_data->img.img, 0, 0);
	return (0);
}
