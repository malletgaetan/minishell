/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbatteux <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 15:57:55 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/11 16:40:23 by tbatteux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int	get_taille(char **env)
{
	int	i;

	i = 0;
	while (env[i])
		i++;
	return (i);
}

int	ft_strlen(char *str)
{
	int	i;
	
	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strdup(const char *s)
{
	char	*final;
	int		i;

	i = 0;
	while (s[i])
		i++;
	final = malloc ((i + 1) * sizeof(char));
	if (!final)
		return (NULL);
	i = 0;
	while (s[i])
	{
		final[i] = s[i];
		i++;
	}
	final[i] = 0;
	return (final);
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t			i;

	if (!dst && !src)
		return (NULL);
	i = 0;
	while (i < n)
	{
		((char *)(dst))[i] = ((char *)src)[i];
		i++;
	}
	return (dst);
}

char	*ft_strjoin(char *s1, char *s2)
{
	size_t	s1len;
	size_t	s2len;
	size_t	allocsize;
	char	*ptr;

	s1len = ft_strlen(s1);
	s2len = ft_strlen(s2);
	allocsize = s1len + s2len + 1;
	ptr = (char *)malloc(sizeof(char) * allocsize);
	if (!ptr)
		return (NULL);
	ft_memcpy(ptr, s1, s1len);
	ft_memcpy(ptr + s1len, s2, s2len);
	*(ptr + s1len + s2len) = '\0';
	return (ptr);
}



