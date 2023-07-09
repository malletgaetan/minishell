NAME = minishell
CC = gcc
SRCSDIR = ./srcs/
HEADERSDIR = includes/
CFLAGS = -Wall -Werror -Wextra -I $(HEADERSDIR)
LIBFTDIR = libft/
LIBFT = libft.a
LIBFTFLAGS = -L $(LIBFTDIR) -l ft
RM = rm -f

SRCS=$(shell find $(SRCSDIR) -name '*.c')
OBJS=$(SRCS:.c=.o)

all: $(NAME)

$(LIBFTDIR)/$(LIBFT):
	make -C $(LIBFTDIR)

$(NAME): $(SRCS) $(LIBFTDIR)/$(LIBFT)
	$(CC) $(CFLAGS) $(SRCS) -o $(NAME) $(LIBFTFLAGS)

clean:
	$(RM) $(NAME)

fclean: clean
	make -C $(LIBFTDIR) fclean

re: fclean all

.PHONY: all clean fclean re
