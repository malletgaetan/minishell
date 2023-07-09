/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmallet <gmallet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 21:53:41 by gmallet           #+#    #+#             */
/*   Updated: 2023/05/01 21:53:42 by gmallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	size_t	allocsize;
	char	*ptr;

	allocsize = sizeof(char) * (ft_strlen(s) + 1);
	ptr = (char *)malloc(allocsize);
	if (!ptr)
		return (NULL);
	ft_strlcpy(ptr, s, allocsize);
	return (ptr);
}
