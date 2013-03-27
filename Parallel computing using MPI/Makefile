CXX= mpic++
CC= mpicc

all: main

main: main.o
	$(CXX) -o main main.o

main.o: main.cpp
	$(CXX) -g -c main.cpp

clean:
	rm -rf *.o main *.cpp.*
