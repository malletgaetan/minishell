/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmallet <gmallet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 01:13:32 by gmallet           #+#    #+#             */
/*   Updated: 2023/05/02 01:13:33 by gmallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	is_specifier(char c)
{
	return (c == 's'
		|| c == 'c'
		|| c == 'p'
		|| c == 'd'
		|| c == 'i'
		|| c == 'u'
		|| c == 'x'
		|| c == 'X'
		|| c == '%');
}

static ssize_t	print_arg(char specifier, va_list *ap)
{
	if (specifier == 'c')
		return (ft_putchar((char)va_arg(*ap, int)));
	else if (specifier == 's')
		return (ft_putstr(va_arg(*ap, char *)));
	else if (specifier == 'p')
		return (ft_putptr(va_arg(*ap, void *)));
	else if (specifier == 'i' || specifier == 'd')
		return (ft_put_signed_nbr(va_arg(*ap, unsigned int)));
	else if (specifier == 'u')
		return (ft_put_unsigned_nbr(va_arg(*ap, unsigned int), 10, 0));
	else if (specifier == 'x')
		return (ft_put_unsigned_nbr(va_arg(*ap, unsigned int), 16, 0));
	else if (specifier == 'X')
		return (ft_put_unsigned_nbr(va_arg(*ap, unsigned int), 16, 1));
	else
		return (ft_putchar('%'));
}

static int	check_ret(ssize_t ret, int c)
{
	if (c == -1 || ret == -1)
		return (-1);
	if (INT_MAX - c < ret)
		return (-1);
	return (c + (int)ret);
}

static int	interpret(const char *format, va_list *args)
{
	int		c;
	size_t	i;

	i = 0;
	c = 0;
	while (format[i])
	{
		if (format[i] == '%' && is_specifier(format[i + 1]))
		{
			c = check_ret(print_arg(format[i + 1], args), c);
			format = &format[i + 2];
			i = 0;
		}
		while (format[i] && format[i] != '%')
			i++;
		c = check_ret(ft_write((char *)format, i), c);
	}
	return (c);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		count;

	if (!format)
		return (0);
	count = 0;
	va_start(args, format);
	count = interpret(format, &args);
	va_end(args);
	return (count);
}
