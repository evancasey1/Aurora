#Makefile for Aurora

CXX = g++
CXXFLAGS = -g -Wall -std=c++14
OBJS = main.o Player.o CursorUtilities.o

aurora: $(OBJS)
	$(CXX) $(CXXFLAGS) -o aurora $(OBJS)

main.o: main.cpp Player.h Structure.h MapSection.h CursorUtilities.h
	$(CXX) $(CXXFLAGS) -c main.cpp
Player.o: Player.cpp Player.h Human.h Elf.h Dwarf.h CursorUtilities.h
	$(CXX) $(CXXFLAGS) -c Player.cpp
CursorUtilities.o: CursorUtilities.cpp CursorUtilities.h
	$(CXX) $(CXXFLAGS) -c CursorUtilities.cpp

clean:
	rm -f $(OBJS) aurora