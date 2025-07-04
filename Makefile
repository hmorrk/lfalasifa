NAME		= philo

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -pthread

SRCS		= main.c init.c monitor.c routine.c utils.c 
OBJS		= $(SRCS:.c=.o)

INCS		= philo.h

all: $(NAME)

$(NAME): $(OBJS) $(INCS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c $(INCS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re