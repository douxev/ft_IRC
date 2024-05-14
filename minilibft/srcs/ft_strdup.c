/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aauthier <aauthier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 22:28:49 by jdoukhan          #+#    #+#             */
/*   Updated: 2024/05/10 02:04:37 by aauthier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*tab;
	int		i;

	i = 0;
	tab = ft_calloc((size_t) ft_strlen((void *) s) + 1, (size_t) sizeof(char));
	if (!tab)
		return (NULL);
	while (s && s[i])
	{
		tab[i] = s[i];
		i++;
	}
	tab[i] = 0;
	return (tab);
}
