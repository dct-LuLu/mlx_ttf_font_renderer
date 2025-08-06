/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:06:47 by jaubry--          #+#    #+#             */
/*   Updated: 2025/08/06 04:03:18 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include "error_handler.h"

/*
        Function that computes the time based on EPOCH
*/
#include <stdio.h>
#include <errno.h>
#include <string.h>

ssize_t	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
	{
		printf("feur\n");
		return (printf("gettimeofday failed: %s\n", strerror(errno)),error(ERR_TIME, NULL), -1);
	}
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}
