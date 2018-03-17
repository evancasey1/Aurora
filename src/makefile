#Makefile for Aurora

CXX = g++
CXXFLAGS = -g -Wall -pedantic -std=c++14
OBJS = main.o player.o map.o enemy.o weapon.o food.o armor.o attack.o
PATHS = -I boost -I Documents/Aurora/src

aurora: $(OBJS)
	$(CXX) $(CXXFLAGS) -o aurora $(OBJS) -lncurses $(PATHS)

main.o: main.cpp player.h map.h equipment.h color.h equipmenttype.h visitors.h
	$(CXX) $(CXXFLAGS) -c main.cpp
player.o: player.cpp player.h equipment.h enemy.h color.h equipmenttype.h visitors.h weaponInstance.h
	$(CXX) $(CXXFLAGS) -c player.cpp
map.o: map.cpp map.h enemy.h player.h
	$(CXX) $(CXXFLAGS) -c map.cpp
enemy.o: enemy.cpp enemy.h weapon.h equipment.h color.h player.h weaponInstance.h map.h
	$(CXX) $(CXXFLAGS) -c enemy.cpp
weapon.o: weapon.cpp weapon.h equipment.h weaponConstants.h color.h attack.h
	$(CXX) $(CXXFLAGS) -c weapon.cpp
food.o: food.cpp food.h equipment.h equipmenttype.h
	$(CXX) $(CXXFLAGS) -c food.cpp
armor.o: armor.cpp armor.h equipment.h armortype.h equipmenttype.h color.h
	$(CXX) $(CXXFLAGS) -c armor.cpp
attack.o: attack.cpp attack.h
	$(CXX) $(CXXFLAGS) -c attack.cpp

clean:
	rm -f $(OBJS) aurora

rebuild:
	make clean && make
