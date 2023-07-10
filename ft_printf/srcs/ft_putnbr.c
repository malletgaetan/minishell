/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmallet <gmallet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 01:13:43 by gmallet           #+#    #+#             */
/*   Updated: 2023/05/02 01:13:44 by gmallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	fill_buf(char *buf, unsigned int nb, int nblen)
{
	int	i;

	i = 0;
	while (i < nblen)
	{
		buf[nblen - 1 - i] = (nb % 10) + '0';
		nb /= 10;
		i++;
	}
}

ssize_t	ft_put_signed_nbr(int nb)
{
	ssize_t			ret;
	char			*buf;
	int				nblen;

	nblen = get_nblen(nb);
	buf = malloc(sizeof(char) * nblen);
	if (!buf)
		return (-1);
	if (nb < 0)
	{
		buf[0] = '-';
		fill_buf(buf + 1, (unsigned int)-nb, nblen - 1);
	}
	else
	{
		fill_buf(buf, (unsigned int)nb, nblen);
	}
	ret = ft_write(buf, nblen);
	free(buf);
	return (ret);
}

ssize_t	ft_put_unsigned_nbr(unsigned int nb, int base, int upper)
{
	ssize_t			ret;
	char			*buf;
	int				nblen;
	int				i;

	nblen = get_unblen(nb, base);
	buf = malloc(sizeof(char) * nblen);
	if (!buf)
		return (-1);
	i = 0;
	while (i < nblen)
	{
		buf[nblen - 1 - i] = ft_hex(nb % base, upper);
		nb /= base;
		i++;
	}
	ret = ft_write(buf, nblen);
	free(buf);
	return (ret);
}
