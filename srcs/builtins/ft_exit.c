/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmallet <gmallet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 17:39:31 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/17 18:19:20 by gmallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_builtin(char **env)
{
	kill_all_child();
	gc_clean(&(g_minishell.gcan));
	exit(0);
}
