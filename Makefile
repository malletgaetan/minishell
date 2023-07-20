NAME = minishell
CC = gcc
SRCSDIR = ./srcs/
HEADERSDIR = includes/
CFLAGS = -Wall -Werror -Wextra -I $(HEADERSDIR)
LIBGCDIR = gc/
LIBFTDIR = libft/
LIBGC = libgc.a
LIBFT = libft.a
LIBFTFLAGS = -L $(LIBFTDIR) -l:$(LIBFT)
LIBGCFLAGS = -L $(LIBGCDIR) -l:$(LIBGC)
RM = rm -f

SRCS=$(shell find $(SRCSDIR) -name '*.c')
OBJS=$(SRCS:.c=.o)

all: $(NAME)

$(LIBGCDIR)/$(LIBGC):
	make -C $(LIBGCDIR)

$(LIBFTDIR)/$(LIBFT):
	make -C $(LIBFTDIR)

$(NAME): $(SRCS) $(LIBFTDIR)/$(LIBFT) $(LIBGCDIR)/$(LIBGC)
	$(CC) $(CFLAGS) $(SRCS) -o $(NAME) $(LIBFTFLAGS) $(LIBGCFLAGS) -lreadline -g -fsanitize=address -fno-omit-frame-pointer

clean:
	$(RM) $(NAME)
	find . -name "*.o" -type f -delete

fclean: clean
	make -C $(LIBFTDIR) fclean
	make -C $(LIBGCDIR) fclean

re: fclean all

.PHONY: all clean fclean re
