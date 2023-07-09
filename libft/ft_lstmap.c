/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmallet <gmallet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 21:52:33 by gmallet           #+#    #+#             */
/*   Updated: 2023/05/02 16:54:11 by gmallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	map_from(t_list *map, t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*cur;
	void	*modified_content;

	cur = map;
	while (lst)
	{
		modified_content = f(lst->content);
		if (!modified_content)
			return (1);
		cur->next = ft_lstnew(modified_content);
		if (!cur->next)
		{
			del(modified_content);
			return (1);
		}
		cur = cur->next;
		lst = lst->next;
	}
	return (0);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*map;
	void	*modified_content;

	if (!lst)
		return (NULL);
	modified_content = f(lst->content);
	if (!modified_content)
		return (NULL);
	map = ft_lstnew(modified_content);
	if (!map)
	{
		del(modified_content);
		return (NULL);
	}
	if (map_from(map, lst->next, f, del))
	{
		ft_lstclear(&map, del);
		return (NULL);
	}
	return (map);
}
