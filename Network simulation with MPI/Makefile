CXX= mpic++
CC= mpicc

all: tema4

tema4: tema4.o
	$(CXX) -o tema4 tema4.o

tema4.o: tema4.cpp Matrix.h Message.h Process.h RoutingTable.h LocalTopology.h GlobalTopology.h
	$(CXX) -g -c tema4.cpp

clean:
	rm -rf *.o tema4 *.cpp.*
