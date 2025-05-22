/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 10:17:42 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/22 01:06:39 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

static bool	is_ttf_ext(char *filename)
{
	size_t	i;

	i = 0;
	while (filename[i])
	{
		if ((filename[i] == '.') && (ft_strncmp(filename + i, ".ttf", 5) == 0))
			return (true);
		i++;
	}
	return (false);
}

int	main(int argc, char **argv)
{
	t_env	*env;

	if (argc == 2)
	{
		if (!is_ttf_ext(argv[1]))
			return (error(ERR_FILE_EXT, ": '%s'", argv[1]));
		env = ft_calloc(sizeof(t_env), 1);
		if (!env)
			return (1);
		if (init_ttf_struct(&env->font))
			free_env(env);
		if (!read_ttf(env->font, argv[1]))
			renderer_mainloop(env);
		free_env(env);
	}
	else
		return (error(ERR_ARG_NUM, ": %d", argc - 1));
	return (0);
}
