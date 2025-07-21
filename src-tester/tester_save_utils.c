/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 16:49:57 by jaubry--          #+#    #+#             */
/*   Updated: 2025/07/16 00:08:07 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

int	save_env_state(t_env *env, const char *filename)
{
	FILE	*file;

	file = fopen(filename, "w");
	if (!file)
		return (-1);
	fprintf(file, "%zu\n", env->cur_pos);
	fprintf(file, "%d\n", (int)env->capslock);
	fprintf(file, "%d\n", (int)env->subpixel);
	fprintf(file, "%d\n", env->x);
	fprintf(file, "%d\n", env->y);
	fprintf(file, "%d\n", env->last_x);
	fprintf(file, "%d\n", env->last_y);
	fprintf(file, "%d\n", env->zoom);
	fprintf(file, "%d\n", env->view_mode);
	fclose(file);
	return (0);
}

int	load_env_state(t_env *env, const char *filename)
{
	FILE	*file;
	int		temp[2];

	file = fopen(filename, "r");
	if (!file)
		return (-1);
	if (fscanf(file, "%zu\n%d\n%d\n", &env->cur_pos, &temp[0], &temp[1]) != 3)
		return (fclose(file), -1);
	env->capslock = (bool)temp[0];
	env->subpixel = (bool)temp[1];
	if (fscanf(file, "%d\n%d\n%d\n%d\n%d\n%d\n", &env->x, &env->y,
			&env->last_x, &env->last_y, &env->zoom, &env->view_mode) != 6)
		return (fclose(file), -1);
	fclose(file);
	return (0);
}
