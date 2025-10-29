/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_file_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 16:49:57 by jaubry--          #+#    #+#             */
/*   Updated: 2025/10/29 05:32:53 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"
#include "tester.h"

void	save_env_state(t_env *env, t_mlx *mlx_data)
{
	FILE	*file;

	(void)mlx_data;
	file = fopen(SAVE_FILENAME, "w");
	if (!file)
		return ;
	fprintf(file, "%d\n", (int)env->subpixel);
	fprintf(file, "%d\n", env->x);
	fprintf(file, "%d\n", env->y);
	fprintf(file, "%d\n", env->last_x);
	fprintf(file, "%d\n", env->last_y);
	fprintf(file, "%d\n", env->zoom);
	fprintf(file, "%d\n", env->view_mode);
	fclose(file);
}

void	load_env_state(t_env *env, t_mlx *mlx_data)
{
	FILE	*file;
	int		temp;

	(void)mlx_data;
	file = fopen(SAVE_FILENAME, "r");
	if (!file)
		return ;
	if (fscanf(file, "%d\n", &temp) != 1)
	{
		fclose(file);
		return ;
	}
	env->subpixel = (bool)temp;
	fscanf(file, "%d\n%d\n%d\n%d\n%d\n%d\n", &env->x, &env->y,
		&env->last_x, &env->last_y, &env->zoom, &env->view_mode);
	fclose(file);
}
