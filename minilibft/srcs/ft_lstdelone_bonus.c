/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aauthier <aauthier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 09:15:56 by jdoukhan          #+#    #+#             */
/*   Updated: 2024/05/10 02:02:49 by aauthier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdelone(t_list **head, t_list *node_to_del)
{
	t_list	*node_prev;
	t_list	*node_next;
	t_list	*temp;

	temp = *head;
	node_prev = NULL;
	while (temp)
	{
		if (temp->next == node_to_del)
		{
			node_prev = temp;
			break ;
		}
		else if (node_to_del == *head)
			node_prev = NULL;
		temp = temp->next;
	}
	node_next = node_to_del->next;
	if (node_prev)
		node_prev->next = node_next;
	if (node_to_del == *head)
		*head = node_next;
	free(node_to_del);
}
