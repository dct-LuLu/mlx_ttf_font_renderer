/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_text.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 21:59:47 by jaubry--          #+#    #+#             */
/*   Updated: 2025/07/17 23:03:17 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

void	add_text(t_rast_env *env, t_text *text)
{
	if (env->text_num == MAX_TEXTS)
		return ;
	env->texts[env->text_num] = text;
	env->text_num++;
}