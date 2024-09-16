/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmallet <gmallet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 10:23:04 by gmallet           #+#    #+#             */
/*   Updated: 2023/07/22 10:23:32 by gmallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static uint32_t	atoi_buf(char *str, unsigned int old_status)
{
	uint32_t	res;

	if (old_status < 10)
	{
		*str = (old_status % 10) + '0';
		return (1);
	}
	res = atoi_buf(str, old_status / 10);
	str[res] = (old_status % 10) + '0';
	return (res + 1);
}

static uint32_t	cpy_old_status(char *str)
{
	if (g_ms.old_status < 0)
	{
		*(str++) = '-';
		return (atoi_buf(str, -(g_ms.old_status)) + 1);
	}
	return ((atoi_buf(str, g_ms.old_status)));
}

static void	cpy_env(char *env, char **word)
{
	if (env == NULL)
		return ;
	while (*env)
		*((*word)++) = *(env++);
}

int	expand(char **line, char **word)
{
	if (!(**line == '$'
			&& (ft_isenv(*((*line) + 1)) || ((*((*line) + 1) == '?')))))
		return (0);
	++(*line);
	if ((**line) == '?')
	{
		++(*line);
		(*word) += cpy_old_status(*word);
		return (1);
	}
	cpy_env(get_env_from_line(line), word);
	return (1);
}
