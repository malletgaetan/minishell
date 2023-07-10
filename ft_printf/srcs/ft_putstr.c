/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmallet <gmallet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 01:13:53 by gmallet           #+#    #+#             */
/*   Updated: 2023/05/02 01:13:54 by gmallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static size_t	ft_strlen(char *str)
{
	size_t	c;

	c = 0;
	while (str[c])
		c++;
	return (c);
}

ssize_t	ft_putstr(char *str)
{
	size_t	c;

	if (!str)
	{
		write(1, "(null)", 6);
		return (6);
	}
	c = ft_strlen(str);
	return (ft_write(str, c));
}
