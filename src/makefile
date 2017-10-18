#Makefile for Aurora

CXX = g++
CXXFLAGS = -g -Wall -std=c++11 
OBJS = main player

aurora: $(OBJS)
	$(CXX) $(CXXFLAGS) -o aurora $(OBJS)

main: main.cpp Player.h
	$(CXX) $(CXXFLAGS) -c main.cpp -o main
player: Player.cpp Player.h Human.h Elf.h Dwarf.h
	$(CXX) $(CXXFLAGS) -c Player.cpp -o player

clean:
	rm -f $(OBJS) aurora