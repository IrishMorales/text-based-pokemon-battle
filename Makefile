# sets compiler to g++
CC := g++
#define objects
OBJECTS = pkmnBattle.cpp pkmn.cpp
# name of .exe file to be created
EXEC = pkmnBattle.exe
# flags to be used
# FLAGS = -Wall

print:
	${CC} -o ${EXEC} ${OBJECTS}