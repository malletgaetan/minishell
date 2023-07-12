NAME = minishell
CC = gcc
SRCSDIR = ./srcs/
HEADERSDIR = includes/
CFLAGS = -Wall -Werror -Wextra -I $(HEADERSDIR)
LIBGCDIR = gc/
FTPRINTFDIR = ft_printf/
LIBFTDIR = libft/
LIBGC = libgc.a
FTPRINTF = libftprintf.a
LIBFT = libft.a
LIBFTFLAGS = -L $(LIBFTDIR) -l:$(LIBFT)
FTPRINTFFLAGS = -L $(FTPRINTFDIR) -l:$(FTPRINTF)
LIBGCFLAGS = -L $(LIBGCDIR) -l:$(LIBGC)
RM = rm -f

SRCS=$(shell find $(SRCSDIR) -name '*.c')
OBJS=$(SRCS:.c=.o)

all: $(NAME)

$(LIBGCDIR)/$(LIBGC):
	make -C $(LIBGCDIR)

$(LIBFTDIR)/$(LIBFT):
	make -C $(LIBFTDIR)

$(FTPRINTFDIR)/$(FTPRINTF):
	make -C $(FTPRINTFDIR)

$(NAME): $(SRCS) $(LIBFTDIR)/$(LIBFT) $(FTPRINTFDIR)/$(FTPRINTF) $(LIBGCDIR)/$(LIBGC)
	$(CC) $(CFLAGS) $(SRCS) -o $(NAME) $(LIBFTFLAGS) $(FTPRINTFFLAGS) $(LIBGCFLAGS) -lreadline

clean:
	$(RM) $(NAME)
	find . -name "*.o" -type f -delete

fclean: clean
	make -C $(LIBFTDIR) fclean
	make -C $(FTPRINTFDIR) fclean
	make -C $(LIBGCDIR) fclean

re: fclean all

.PHONY: all clean fclean re
