/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmallet <gmallet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 08:36:09 by gmallet           #+#    #+#             */
/*   Updated: 2023/07/22 08:36:10 by gmallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_exit(int err)
{
	gc_clean(&(g_ms.gcan));
	gc_clean(&(g_ms.gcenv));
	exit(err);
}

void	hardfail_exit(int lerrno)
{
	ft_putstr_fd("minishell: internal error: ", STDERR_FILENO);
	ft_putstr_fd(strerror(lerrno), STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	kill_all_childs(SIGKILL);
	wait_all_childs();
	gc_clean(&(g_ms.gcan));
	gc_clean(&(g_ms.gcenv));
	exit(lerrno);
}
