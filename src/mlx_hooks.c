/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_hooks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 22:32:25 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/29 16:06:38 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

/*
	Function that safely kills the mlx instance.
*/
int	kill_mlx(t_mlx *mlx)
{
	if (mlx)
	{
		if (mlx->mlx)
		{
			if (mlx->img.img)
				mlx_destroy_image(mlx->mlx, mlx->img.img);
			if (mlx->win)
				mlx_destroy_window(mlx->mlx, mlx->win);
			mlx_destroy_display(mlx->mlx);
			free(mlx->mlx);
		}
		free(mlx);
	}
	return (0);
}

void	edit_text(int keysym, t_env *env)
{
	if (keysym == 65509)
		env->capslock = !env->capslock;
	if (keysym == 65288 && env->cur_pos > 0)
	{
		env->text[env->cur_pos - 1] = 0;
		env->cur_pos--;
	}
	if (env->cur_pos < 199)
	{
		if (ft_isprint(keysym))
			env->text[env->cur_pos] = keysym + env->capslock * -32 * ft_isalpha(keysym);
		else if (keysym == 65293)
			env->text[env->cur_pos] = '\n';
		else
			return ;
		if (DEBUG)
			printf("%d: '%c'\n", keysym, keysym);
		env->cur_pos++;
	}
}

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

/*
	Function to handles ESC key to exit safely.
*/
int	on_key_press(int keysym, t_env *env)
{
	if (env->view_mode == 2)
		edit_text(keysym, env);
	if (keysym == 65307)
		return (mlx_loop_end(env->mlx->mlx));
	else if (keysym == XK_Left)
		env->x += 200;
	else if (keysym == XK_Up)
		env->y += 200;
	else if (keysym == XK_Right)
		env->x -= 200;
	else if (keysym == XK_Down)
		env->y -= 200;
	else if (keysym == XK_v)
		env->view_mode = (1 + env->view_mode) % 3;
	else if (keysym == XK_p)
		env->subpixel = !env->subpixel;
	else if (keysym == XK_s)
		save_env_state(env, ".env_save.txt");
	else if (keysym == XK_l)
		load_env_state(env, ".env_save.txt");
	return (0);
}

int	on_mwheel_drag(int x, int y, t_env *env)
{
	if (env->zoom <= 0)
	{
		env->x += (x - env->last_x) / ((-env->zoom) + 1);
		env->y += (y - env->last_y) / ((-env->zoom) + 1);
	}
	else
	{
		env->x += env->zoom * (x - env->last_x);
		env->y += env->zoom * (y - env->last_y);
	}
	if (DEBUG)
	{
		if ((x - env->last_x) > 0)
			printf("+");
		printf("%d\t", (x - env->last_x));
		if ((y - env->last_y) > 0)
			printf("+");
		printf("%d\n", (y - env->last_y));
	}
	env->last_x = x;
	env->last_y = y;
	return (0);
}

int	on_button_press(int mousecode, int x, int y, t_env *env)
{
	if (mousecode == 2)
	{
		env->last_x = x;
		env->last_y = y;
	}
	if (mousecode == 5)
		env->zoom += 1;
	else if (mousecode == 4)
		env->zoom -= 1;
	//printf("x:%d y:%d\t%d\n", x, y, mousecode);
	return (0);
}

int	on_button_release(int mousecode, int x, int y, t_env *env)
{
	//printf("x:%d y:%d\t%d\n", x, y, mousecode);
	(void)x;
	(void)y;
	(void)mousecode;
	(void)env;
	return (0);
}

int	mouse_handler(int mousecode, int x, int y, t_env *env)
{
	(void)mousecode;
	(void)env;
	(void)x;
	(void)y;
	//printf("dddddx:%d y:%d\t%d\n", x, y, mousecode);
	return (0);
}
