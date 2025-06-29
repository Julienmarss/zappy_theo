##
## EPITECH PROJECT, 2024
## zappy
## File description:
## Makefile
##

NAME = ../zappy_ai

all: $(NAME)

$(NAME): main.py
	@echo '#!/bin/bash' > $(NAME)
	@echo '' >> $(NAME)
	@echo 'python3 IA/main.py "$$@"' >> $(NAME)
	@chmod +x $(NAME)

clean:
	echo "Nothing to clean in Python"

fclean: clean
	@rm -f $(NAME)

re: fclean all
