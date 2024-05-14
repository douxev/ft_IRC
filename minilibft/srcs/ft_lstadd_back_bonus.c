/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdoukhan <jdoukhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 09:15:52 by jdoukhan          #+#    #+#             */
/*   Updated: 2024/05/08 21:17:23 by jdoukhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	if (new->content && !((char *)new->content)[0])
	{
		free(new->content);
		free(new);
		return ;
	}
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	(ft_lstlast(*lst))->next = new;
	new->next = NULL;
}
