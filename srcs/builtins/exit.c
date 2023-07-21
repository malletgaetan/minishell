/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmallet <gmallet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 17:39:31 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/17 19:14:24 by gmallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_builtin(void)
{
	kill_all_childs(SIGKILL, 0);
	wait_all_childs();
	gc_clean(&(g_ms.gcan));
	gc_clean(&(g_ms.gcenv));
	exit(0);
}
