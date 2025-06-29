##
## EPITECH PROJECT, 2024
## zappy
## File description:
## Makefile
##

SRC	=	src/main.c \
		src/socket.c \
		src/signal.c \
		src/set.c \
		src/utils.c \
		src/tick.c \
		src/map_generate.c \
		src/gui_cmds.c \
		src/ia/config_ia.c \
		src/ia/mort.c \
		src/ia/command.c \
		src/ia/inventory.c \
		src/ia/mouvement.c \
		src/ia/vision.c \
		src/ia/take_object.c \
		src/ia/ejection.c \
		src/ia/set_object.c \
		src/gui_setup.c \
		src/queue.c \
		src/ia/incantation.c \
		src/ia/fork.c \
		src/ia/eject.c \
		src/config_socket.c \
		src/regen_resources.c \
		src/end_game.c \
		src/ia/broadcast_utils.c \
		src/utils_socket.c \
		src/create_maps_content.c

NAME	=	../zappy_server

OBJ	= $(SRC:.c=.o)

CFLAGS	= -I./include/
LDFLAGS	= -luuid -lm

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	CFLAGS += -I/opt/homebrew/include
	LDFLAGS += -L/opt/homebrew/lib
endif

all: $(NAME)

$(NAME):	$(OBJ)
	$(CC) -g $(OBJ) -o $(NAME) $(CFLAGS) $(LDFLAGS)

debug:	fclean $(OBJ)
	gcc -g $(SRC) -o $(NAME) $(CFLAGS) $(LDFLAGS)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)
	rm -rf vgcore*
	rm -rf ../vgcore*

re: fclean all
