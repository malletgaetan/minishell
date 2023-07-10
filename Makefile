NAME = minishell
CC = gcc
SRCSDIR = ./srcs/
HEADERSDIR = includes/
CFLAGS = -Wall -Werror -Wextra -I $(HEADERSDIR)
FTPRINTFDIR = ft_printf/
LIBFTDIR = libft/
FTPRINTF = libftprintf.a
LIBFT = libft.a
LIBFTFLAGS = -L $(LIBFTDIR) -l ft
FTPRINTFFLAGS = -L $(FTPRINTFDIR) -l ftprintf
RM = rm -f

SRCS=$(shell find $(SRCSDIR) -name '*.c')
OBJS=$(SRCS:.c=.o)

all: $(NAME)

$(LIBFTDIR)/$(LIBFT):
	make -C $(LIBFTDIR)

$(FTPRINTFDIR)/$(FTPRINTF):
	make -C $(FTPRINTFDIR)

$(NAME): $(SRCS) $(LIBFTDIR)/$(LIBFT) $(FTPRINTFDIR)/$(FTPRINTF)
	$(CC) $(CFLAGS) $(SRCS) -o $(NAME) $(LIBFTFLAGS) $(FTPRINTFFLAGS)

clean:
	$(RM) $(NAME)
	$(RM) **/*.o

fclean: clean
	make -C $(LIBFTDIR) fclean
	make -C $(FTPRINTFDIR) fclean

re: fclean all

.PHONY: all clean fclean re
