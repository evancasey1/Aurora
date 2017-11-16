#Makefile for Aurora

CXX = g++
CXXFLAGS = -g -Wall -pedantic -std=c++14
OBJS = main.o player.o map.o enemy.o weapon.o

aurora: $(OBJS)
	$(CXX) $(CXXFLAGS) -o aurora $(OBJS) -lncurses

main.o: main.cpp player.h map.h
	$(CXX) $(CXXFLAGS) -c main.cpp
player.o: player.cpp player.h
	$(CXX) $(CXXFLAGS) -c player.cpp
map.o: map.cpp map.h enemy.h player.h
	$(CXX) $(CXXFLAGS) -c map.cpp
enemy.o: enemy.cpp enemy.h
	$(CXX) $(CXXFLAGS) -c enemy.cpp
weapon.o: weapon.cpp weapon.h
	$(CXX) $(CXXFLAGS) -c weapon.cpp

clean:
	rm -f $(OBJS) aurora