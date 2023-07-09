/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmallet <gmallet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 21:53:21 by gmallet           #+#    #+#             */
/*   Updated: 2023/05/02 15:52:18 by gmallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static unsigned int	fill_number(char *buf, unsigned int nb)
{
	*buf = (nb % 10) + '0';
	if ((nb / 10) == 0)
		return (1);
	return (1 + fill_number(buf - 1, nb / 10));
}

void	ft_putnbr_fd(int nb, int fd)
{
	char			tab[11];
	unsigned int	nblen;

	if (nb < 0)
	{
		nblen = fill_number(tab + 10, (unsigned int)-nb);
		tab[10 - (nblen++)] = '-';
	}
	else
	{
		nblen = fill_number(tab + 10, (unsigned int)nb);
	}
	write(fd, tab + 11 - nblen, nblen);
}
