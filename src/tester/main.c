/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 00:55:28 by jaubry--          #+#    #+#             */
/*   Updated: 2025/10/29 02:51:33 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"
#include "tester.h"

int		init_ttf(const char *path, t_ttf_font **ptr);
void	start_mainloop(t_env *env);
void	free_ttf(t_ttf_font *font);

int	main(int argc, char **argv)
{
	t_env		env;
	int			ret;

	if (argc == 2)
	{
		printf("'%s'\n", argv[1]);
		ft_bzero(&env, sizeof(t_env));
		ret = init_ttf(argv[1], &env.font);
		if (ret)
			return (ret);
		start_mainloop(&env);
		free_ttf(env.font);
	}
	else
	{
		printf("Invalid argument number\n");
		return (1);
	}
	return (0);
}
