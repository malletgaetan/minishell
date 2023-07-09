/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmallet <gmallet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 21:50:18 by gmallet           #+#    #+#             */
/*   Updated: 2023/05/01 21:50:19 by gmallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;
	int		allocsize;

	if (count && (size > (SIZE_MAX / count)))
		return (NULL);
	allocsize = count * size;
	ptr = malloc(allocsize);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, allocsize);
	return (ptr);
}
