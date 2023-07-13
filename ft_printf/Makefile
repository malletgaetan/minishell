NAME = libftprintf.a
CC = gcc
SRCSDIR = srcs/
HEADERSDIR = includes/
CFLAGS = -Wall -Werror -Wextra -I $(HEADERSDIR)
AR = ar rcs
RM = rm -f

_SRCS = ft_printf.c \
		ft_putchar.c \
		ft_putnbr.c \
		ft_putstr.c \
		ft_putptr.c \
		utils.c

SRCS = $(addprefix $(SRCSDIR), $(_SRCS))
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $(<:.c=.o)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
