all: main.o definition.o bfs.o SCC.o UnitTesting.o
	gcc -o pro main.o definition.o bfs.o SCC.o UnitTesting.o -g

main.o: main.c
	gcc -g -c main.c

definition.o: definition.c
	gcc -g -c definition.c

UnitTesting.o: UnitTesting.c
	gcc -g -c UnitTesting.c

bfs.o: bfs.c
	gcc -g -c bfs.c

SCC.o: SCC.c
	gcc -g -c SCC.c