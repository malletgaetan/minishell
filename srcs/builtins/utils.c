/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmallet <gmallet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 15:57:55 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/17 18:14:23 by gmallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_size(char **env)
{
	int	i;

	i = 0;
	while (env[i])
		i++;
	return (i);
}

char	*gc_strdup(char *str)
{
	size_t	i;
	char	*ret;

	if (str == NULL)
		return (NULL);
	i = 0;
	ret = gc_malloc(&(g_minishell.gcenv), (ft_strlen(str) + 1));
	if (ret == NULL)
		return (NULL);
	while (str[i])
	{
		ret[i] = str[i];
		++i;
	}
	ret[i] = '\0';
	return (ret);
}
