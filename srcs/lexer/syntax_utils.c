/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbatteux <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 16:45:22 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/20 16:45:23 by tbatteux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isspace(char c)
{
	return ((c == ' ') || ((c > 8) && (c < 14)));
}

int	ft_isoperator(char c)
{
	return ((c == '<') || (c == '>') || (c == '|')
		|| (c == '&') || (c == '(') || (c == ')'));
}

int	ft_isquote(char c)
{
	return ((c == '\'') || (c == '\"'));
}

int	ft_isenv(char c)
{
	return (ft_isalpha(c) || ft_isdigit(c) || (c == '_'));
}
