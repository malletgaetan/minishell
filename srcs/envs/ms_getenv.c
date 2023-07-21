/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_getenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbatteux <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 16:35:30 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/20 16:35:31 by tbatteux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ms_getenv(char *envname)
{
	size_t	en_len;
	size_t	i;

	en_len = ft_strlen(envname);
	if (en_len == 0)
		return (NULL);
	i = 0;
	while (g_ms.envs[i])
	{
		if (!ft_strncmp(envname, g_ms.envs[i], en_len - 1))
			return (g_ms.envs[i] + en_len + 1);
		++i;
	}
	return (NULL);
}
