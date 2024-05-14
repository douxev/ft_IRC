/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdoukhan <jdoukhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 08:49:18 by jdoukhan          #+#    #+#             */
/*   Updated: 2024/05/08 21:16:09 by jdoukhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*next;

	next = malloc(sizeof(t_list));
	if (!next)
		return (NULL);
	next->next = NULL;
	next->content = content;
	return (next);
}
