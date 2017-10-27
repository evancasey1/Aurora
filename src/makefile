#Makefile for Aurora

CXX = g++
CXXFLAGS = -g -Wall -std=c++14
OBJS = main.o player.o map.o

aurora: $(OBJS)
	$(CXX) $(CXXFLAGS) -o aurora $(OBJS) -lncurses

main.o: main.cpp player.h structure.h map.h
	$(CXX) $(CXXFLAGS) -c main.cpp
player.o: player.cpp player.h human.h elf.h dwarf.h
	$(CXX) $(CXXFLAGS) -c player.cpp
map.o: map.cpp map.h
	$(CXX) $(CXXFLAGS) -c map.cpp

clean:
	rm -f $(OBJS) aurora