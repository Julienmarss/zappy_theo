##
## EPITECH PROJECT, 2024
## zappy
## File description:
## Makefile
##

SRC = src/main.cpp	\
      src/game.cpp	\
      src/gui_protocol.cpp	\
      src/raylib_wrapper.cpp \
      src/Player.cpp \
      src/Team.cpp \
      src/Egg.cpp \

CC = g++
CFLAGS = -I./include -std=c++20
OBJ = $(SRC:.cpp=.o)

# Default paths and flags for Linux
LDFLAGS = -L/usr/local/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Detect the operating system
UNAME_S := $(shell uname -s)

# Adjust paths and flags for macOS
ifeq ($(UNAME_S), Darwin)
    RAYLIB_PATH = /opt/homebrew/opt/raylib
    CFLAGS += -I$(RAYLIB_PATH)/include
    LDFLAGS = -L$(RAYLIB_PATH)/lib -lraylib -framework OpenGL -framework \
     Cocoa -framework IOKit -framework CoreVideo
endif

NAME = ../zappy_gui

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

debug: fclean $(OBJ)
	$(CC) -g $(OBJ) -o $(NAME) $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
