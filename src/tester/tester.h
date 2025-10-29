/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tester.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 01:41:50 by jaubry--          #+#    #+#             */
/*   Updated: 2025/10/29 05:32:14 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TESTER_H
# define TESTER_H

# define MIN_CHAR "       '    ,-./0123456789 ; =                             [\\]  `abcdefghijklmnopqrstuvwxyz"
# define MAX_CHAR "       \"    <_>?)!@#$%^&*( : +                             {|}  ~ABCDEFGHIJKLMNOPQRSTUVWXYZ"
# define NUMPAD_CHAR "7486293150.          *+ - /"

# define SAVE_FILENAME ".env_save.txt"

# define BASIC_VIEW 0
# define FULL_VIEW 1
# define EDIT_VIEW 2

typedef struct s_env
{
	t_mlx		*mlx;
	t_ttf_font	*font;
	t_text		*text;
	t_text		*fps;
	size_t		cur_pos;
	bool		subpixel;
	int			x;
	int			y;
	int			last_x;
	int			last_y;
	int			zoom;
	int			view_mode;
}	t_env;

void	save_env_state(t_env *env, t_mlx *mlx_data);
void	load_env_state(t_env *env, t_mlx *mlx_data);

#endif//TESTER_H
