NAME = minishell
CC = gcc
SRCSDIR = ./srcs/
HEADERSDIR = .
CFLAGS = -Wall -Werror -Wextra -I $(HEADERSDIR)
AR = ar rcs
RM = rm -f

SRCS=$(shell find $(SRCSDIR) -name '*.c')
OBJS=$(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(SRCS) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re