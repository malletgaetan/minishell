/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmallet <gmallet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 21:54:35 by gmallet           #+#    #+#             */
/*   Updated: 2023/05/01 21:54:36 by gmallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_in_set(char c, char const *set)
{
	while (*set)
	{
		if (*(set++) == c)
			return (1);
	}
	return (0);
}

char	*ft_strtrim(char const *s, char const *set)
{
	size_t	allocsize;
	char	*str;

	while (is_in_set(*s, set))
		s++;
	allocsize = ft_strlen(s) + 1;
	while (allocsize > 1 && is_in_set(s[allocsize - 2], set))
		allocsize--;
	str = malloc(sizeof(char) * allocsize);
	if (!str)
		return (NULL);
	ft_memcpy(str, s, allocsize - 1);
	str[allocsize - 1] = '\0';
	return (str);
}
