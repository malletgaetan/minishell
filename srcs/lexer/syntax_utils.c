/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmallet <gmallet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 16:45:22 by tbatteux          #+#    #+#             */
/*   Updated: 2024/01/19 01:37:30 by gmallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isspace(char c)
{
	return ((c == ' ') || ((c > 8) && (c < 14)));
}

int	ft_isoperator(char c)
{
	return (c == '<') || (c == '>') || (c == '|');
}

int	ft_isquote(char c)
{
	return ((c == '\'') || (c == '\"'));
}

int	ft_isenv(char c)
{
	return (ft_isalpha(c) || ft_isdigit(c) || (c == '_'));
}
