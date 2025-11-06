/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_routine.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:39:37 by jaubry--          #+#    #+#             */
/*   Updated: 2025/11/06 11:01:06 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

void	update_fps(t_mlx *mlx_data,t_text *fps);
void	render_text_test(t_text *text);

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
	//ft_mlx_line_put(&env->mlx_data->img, env->mlx_data->origin, env->mlx_data->size, (t_rgb_int){.rgb=0xFFFFFF});
	//ft_mlx_line_put(&env->mlx_data->img, vec2i(env->mlx_data->size.x, env->mlx_data->origin.y), vec2i(env->mlx_data->origin.x, env->mlx_data->size.y), (t_rgb_int){.rgb=0xFFFFFF});
	update_fps(env->mlx_data, env->fps);
	if (env->test->_rb_limit.x - env->test->_lt_limit.x == 500)
		env->test->_rb_limit = env->test->_lt_limit;
	else
		env->test->_rb_limit = vec2i_add_scalar(env->test->_rb_limit, 1);
	ft_mlx_out_aarec_aput(&env->mlx_data->img, env->test->_lt_limit, env->test->_rb_limit, env->test->fg);
	ft_mlx_line_put(&env->mlx_data->img, env->test->_lt_limit, env->test->_rb_limit, (t_rgb_int){.rgb=0xFFFFFF});
	ft_mlx_line_put(&env->mlx_data->img, vec2i(env->test->_rb_limit.x, env->test->_lt_limit.y), vec2i(env->test->_lt_limit.x, env->test->_rb_limit.y), (t_rgb_int){.rgb=0xFFFFFF});
	int	half_x = env->test->_lt_limit.x + (env->test->_rb_limit.x - env->test->_lt_limit.x) / 2;
	ft_mlx_line_put(&env->mlx_data->img, vec2i(half_x, env->test->_lt_limit.y), vec2i(half_x, env->test->_rb_limit.y), (t_rgb_int){.rgb=0xFFFFFF});
	render_text_test(env->test);
	render_text_test(env->fps);
	mlx_put_image_to_window(env->mlx_data->mlx, env->mlx_data->win, env->mlx_data->img.img, 0, 0);
	return (0);
}
