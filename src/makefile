#Makefile for Aurora

CXX = g++
CXXFLAGS = -g -Wall -pedantic -std=c++14
OBJS = main.o player.o map.o enemy.o weapon.o food.o armor.o

aurora: $(OBJS)
	$(CXX) $(CXXFLAGS) -o aurora $(OBJS) -lncurses

main.o: main.cpp player.h map.h equipment.h colors.h
	$(CXX) $(CXXFLAGS) -c main.cpp
player.o: player.cpp player.h equipment.h enemy.h colors.h
	$(CXX) $(CXXFLAGS) -c player.cpp
map.o: map.cpp map.h enemy.h player.h
	$(CXX) $(CXXFLAGS) -c map.cpp
enemy.o: enemy.cpp enemy.h weapon.h equipment.h colors.h
	$(CXX) $(CXXFLAGS) -c enemy.cpp
weapon.o: weapon.cpp weapon.h equipment.h
	$(CXX) $(CXXFLAGS) -c weapon.cpp
food.o: food.cpp food.h equipment.h
	$(CXX) $(CXXFLAGS) -c food.cpp
armor.o: armor.cpp armor.h equipment.h armortype.h
	$(CXX) $(CXXFLAGS) -c armor.cpp

clean:
	rm -f $(OBJS) aurora
