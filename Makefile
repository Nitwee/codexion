NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
RM = rm -f

SRC_DIR = coders

SRCS = $(SRC_DIR)/main.c \
	$(SRC_DIR)/parse.c \
	$(SRC_DIR)/init.c \
	$(SRC_DIR)/init_utils.c \
	$(SRC_DIR)/coder.c \
	$(SRC_DIR)/dongle.c \
	$(SRC_DIR)/dongle_utils.c \
	$(SRC_DIR)/monitor.c \
	$(SRC_DIR)/log.c \
	$(SRC_DIR)/time.c \
	$(SRC_DIR)/heap.c \
	$(SRC_DIR)/priority_queue.c \
	$(SRC_DIR)/cleanup.c

OBJS = $(SRCS:.c=.o)

HEADER = $(SRC_DIR)/codexion.h

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
