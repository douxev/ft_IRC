/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aauthier <aauthier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/15 11:30:13 by aauthier          #+#    #+#             */
/*   Updated: 2024/04/24 18:03:31 by aauthier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minilibft.h"

char	*ft_strcpy(char *dest, const char *src)
{
	int	i;

	i = 0;
	while (src && src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strcat(char *dest, const char *src)
{
	int	i;
	int	pos;

	i = 0;
	pos = ft_strlen(dest);
	while (src && src[i])
	{
		dest[pos] = src[i];
		pos++;
		i++;
	}
	dest[pos] = '\0';
	return (dest);
}

// char	*ft_strjoin(const char *s1, const char *s2)
// {
// 	size_t	str_size;
// 	char	*result;

// 	str_size = ft_strlen(s1) + ft_strlen(s2) + 1;
// 	result = malloc(sizeof(char) * str_size);
// 	if (!result)
// 		return (NULL);
// 	ft_strcpy(result, s1);
// 	ft_strcat(result, s2);
// 	return (result);
// }

// char	*ft_strchr(const char *s, int c)
// {
// 	int		i;
// 	char	*s_char;

// 	i = -1;
// 	if (!s)
// 		return (NULL);
// 	s_char = (char*)s;
// 	if (c == '\0')
// 		return (&s_char[ft_strlen(s)]);
// 	while (s_char && s_char[++i])
// 		if (s_char[i] == (char)c)
// 			return (&s_char[i]);
// 	return (NULL);
// }

// size_t	ft_strlen(const char *s)
// {
// 	size_t	i;

// 	i = 0;
// 	while (s && s[i])
// 		i++;
// 	return (i);
// }
