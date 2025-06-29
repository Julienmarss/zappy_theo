##
## EPITECH PROJECT, 2024
## zappy
## File description:
## Makefile
##

all:
	make -C server/
	make -C GUI/
	make -C IA/

zappy_server :
	make -C server/

zappy_gui :
	make -C GUI/

zappy_ai :
	make -C IA/

clean:
	make clean -C server/
	make clean -C GUI/
	make clean -C IA/


fclean: clean
	make fclean -C server/
	make fclean -C GUI/
	make fclean -C IA/

re:	fclean all
