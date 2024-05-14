/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aauthier <aauthier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/15 11:29:48 by aauthier          #+#    #+#             */
/*   Updated: 2024/05/08 20:00:25 by aauthier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "minilibft.h"

static char	*ft_strndup_static(const char *s, size_t n)
{
	char	*cpy_s;
	size_t	size_s;
	size_t	size;
	size_t	i;

	i = 0;
	size_s = ft_strlen(s);
	size = n;
	if (size_s < n)
		size = size_s;
	cpy_s = malloc(sizeof(char) * (size + 1));
	if (!cpy_s)
		return (NULL);
	while (i < size)
	{
		cpy_s[i] = s[i];
		i++;
	}
	cpy_s[i] = '\0';
	return (cpy_s);
}

static char	*ft_del_line(char *buffer)
{
	char	*newbuffer;

	newbuffer = ft_strndup_static(ft_strchr(buffer, '\n') + 1,
			ft_strlen(buffer));
	free(buffer);
	return (newbuffer);
}

static int	fill_buffer(char **buffer, char *buf_temp)
{
	char	*buf_to_free;

	if (!*buffer)
	{
		*buffer = malloc(sizeof(char) * ft_strlen(buf_temp) + 1);
		if (!buffer)
			return (0);
		ft_strcpy(*buffer, buf_temp);
	}
	else
	{
		buf_to_free = *buffer;
		*buffer = ft_strjoin(buf_to_free, buf_temp);
		free(buf_to_free);
		if (!*buffer)
			return (0);
	}
	return (1);
}

static int	gnl_read_return_zero(char **line, char **buffer)
{
	if (!*buffer)
	{
		*line = ft_strndup_static("", 0);
		if (!*line)
			return (-1);
	}
	else
	{
		*line = *buffer;
		*buffer = NULL;
	}
	return (0);
}

int	get_next_line(int fd, char **line)
{
	static char	*buffer = NULL;
	char		buf_temp[BUFFER_SIZE + 1];
	int			read_size;

	while (!buffer || !ft_strchr(buffer, '\n'))
	{
		read_size = read(fd, buf_temp, BUFFER_SIZE);
		if (read_size == 0)
			return (gnl_read_return_zero(line, &buffer));
		if (read_size == -1)
			return (read_size);
		buf_temp[read_size] = '\0';
		if (!fill_buffer(&buffer, buf_temp))
			return (-1);
	}
	if (buffer)
	{
		*line = ft_strndup_static(buffer, ft_strchr(buffer, '\n') - buffer);
		buffer = ft_del_line(buffer);
		if (buffer[0] == '\0')
			(free(buffer), buffer = NULL);
	}
	else if (!buffer)
		return (-1);
	return (1);
}
