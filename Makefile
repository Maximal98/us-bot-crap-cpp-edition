all: main

main: main.o
	g++ -o rushhour main.o


main.o: main.cpp
	g++ -c main.cpp