/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 10:17:42 by jaubry--          #+#    #+#             */
/*   Updated: 2025/05/15 10:22:37 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "parser_font_ttf.h"
#include "libft.h"

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
	if (argc == 2)
	{
		if (!is_ttf_ext(argv[1]))
			return (error(ERR_FILE_EXT, ": '%s'", argv[1]));
		//access here
		return (read_ttf(argv[1]));
	}
	return (error(ERR_ARG_NUM, ": %d", argc - 1));
}
