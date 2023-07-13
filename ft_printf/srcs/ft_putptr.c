/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmallet <gmallet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 01:13:48 by gmallet           #+#    #+#             */
/*   Updated: 2023/05/02 01:13:49 by gmallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	get_ptrlen(uintptr_t ptr)
{
	int	c;

	c = 0;
	while (ptr)
	{
		c++;
		ptr /= 16;
	}
	return (c);
}

ssize_t	ft_putptr(void *ptr)
{
	uintptr_t	p;
	ssize_t		ret;
	char		*buf;
	int			ptrlen;
	int			i;

	if (!ptr)
		return ((int)write(1, "(nil)", 5));
	p = (uintptr_t)ptr;
	ptrlen = get_ptrlen(p);
	buf = malloc(sizeof(char) * (ptrlen + 2));
	if (!buf)
		return (-1);
	i = 0;
	buf[0] = '0';
	buf[1] = 'x';
	while (i < ptrlen)
	{
		buf[ptrlen + 1 - i] = ft_hex(p % 16, 0);
		p /= 16;
		i++;
	}
	ret = ft_write(buf, ptrlen + 2);
	free(buf);
	return (ret);
}
