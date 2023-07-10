/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbatteux <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 15:57:55 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/10 15:58:15 by tbatteux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

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

