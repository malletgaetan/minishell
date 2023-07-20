/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbatteux <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 16:42:09 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/20 16:42:28 by tbatteux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_piped_builtin(char *cmd)
{
	if (!strcmp(cmd, "echo"))
		return (1);
	if (!strcmp(cmd, "env"))
		return (1);
	if (!strcmp(cmd, "pwd"))
		return (1);
	return (0);
}
