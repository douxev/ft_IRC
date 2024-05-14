/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_wout_wspace.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aauthier <aauthier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 17:37:35 by jdoukhan          #+#    #+#             */
/*   Updated: 2024/05/10 15:18:39 by aauthier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**ft_free(char **tab)
{
	size_t	i;

	i = 0;
	while (tab && tab[i])
		free(tab[i++]);
	if (tab)
		free(tab);
	return (NULL);
}

int	ft_isspace(int c)
{
	if (c == ' ' || (c >= '\t' && c <= '\r'))
		return (1);
	return (0);
}

static size_t	ft_countwords(char const *str)
{
	size_t	i;
	size_t	words;

	i = 0;
	words = 0;
	while (str[i])
	{
		while (ft_isspace(str[i]))
			i++;
		if (!ft_isspace(str[i]) && str[i])
			words++;
		while (!ft_isspace(str[i]) && str[i])
			i++;
	}
	return (words);
}

// A split version to trim the whitespaces and make them the delimiter
char	**ft_split_wout_wspace(char const *s)
{
	char	**tab;
	size_t	i;
	size_t	j;
	size_t	k;

	i = 0;
	k = 0;
	if (!s)
		return (NULL);
	tab = ft_calloc((ft_countwords(s) + 1), sizeof(char *));
	if (!tab)
		return (NULL);
	while (k < ft_countwords(s))
	{
		while (ft_isspace(s[i]))
			i++;
		j = i;
		while (!ft_isspace(s[i]) && s[i])
			i++;
		tab[k] = ft_substr(s, j, i - j);
		if (!tab[k++])
			return (ft_free(tab));
	}
	tab[k] = NULL;
	return (tab);
}
