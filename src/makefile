#Makefile for Aurora

CXX = g++
CXXFLAGS = -g -Wall -std=c++11 
OBJS = main.o Player.o

aurora: $(OBJS)
	$(CXX) $(CXXFLAGS) -o aurora $(OBJS)

main.o: main.cpp Player.h Structure.h MapSection.h
	$(CXX) $(CXXFLAGS) -c main.cpp
Player.o: Player.cpp Player.h Human.h Elf.h Dwarf.h
	$(CXX) $(CXXFLAGS) -c Player.cpp

clean:
	rm -f $(OBJS) aurora