SERVER		=	server
CLIENT		=	client
CC			=	gcc
FLAGS		?=	-Wall -Wextra -Werror
FLAGS		+=	-I./include/ -I./$(LIBFT_DIR)/include/
LIBFT_DIR	=	libft
LIBFT		=	$(LIBFT_DIR)/libft.a
FILENAMES	=	client.c \
				server.c

all: $(CLIENT) $(SERVER)

$(CLIENT): $(LIBFT) client.o
	$(CC) $(FLAGS) -o $@ client.o $(LIBFT)

$(SERVER): $(LIBFT) server.o
	$(CC) $(FLAGS) -o $@ server.o $(LIBFT)

$(LIBFT):
	make -C $(LIBFT_DIR)/

%.o: ./src/%.c
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -rf *.o
	make -C $(LIBFT_DIR)/ fclean

fclean: clean
	rm -f $(CLIENT)
	rm -f $(SERVER)

re: fclean all