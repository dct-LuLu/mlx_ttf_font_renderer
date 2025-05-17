/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   font_renderer.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 23:07:22 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/17 01:06:13 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FONT_RENDERER_H
# define FONT_RENDERER_H

# include "parser_font_ttf.h"
# include "mlx_utils.h"
# include "libft.h"

typedef struct s_env
{
	t_mlx		*mlx;
	t_ttf_font	*font;
}	t_env;

void	*renderer_mainloop(t_env *env);
int		draw_routine(t_env *env);

#endif //FONT_RENDERER_H
