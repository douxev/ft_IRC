/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minilibft.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aauthier <aauthier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 14:27:13 by jdoukhan          #+#    #+#             */
/*   Updated: 2024/05/10 02:02:03 by aauthier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINILIBFT_H
# define MINILIBFT_H

# include <stdio.h>
# include "srcs/libft.h"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 32
# endif

int		ft_printf(const char *str, ...);
int		get_next_line(int fd, char **line);

#endif
