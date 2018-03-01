#include <string>
#include <fstream>
#include <cmath>
#include <math.h>
#include "attack.h"
#include <iostream>
#include <algorithm>
#include <string> 
#include "ncurses.h"

Attack::Attack() {
	this->name = "Attack";
	this->type = "PS";
	this->damage_mod = 1.00;
	this->accuracy_mod = 1.00;
	this->bleed_chance_mod = 1.00;
	this->bleed_rounds = 1;
	this->bleed_damage = 1;
	this->stun_chance_mod = 1.00;
	this->crit_chance_mod = 1.00;
}

Attack::Attack(std::string a_name) {
	this->name = a_name;
    std::transform(a_name.begin(), a_name.end(), a_name.begin(), ::tolower);
	parseCSV(a_name);
}

void Attack::parseCSV(std::string a_name) {
	std::ifstream infile("attackAttributes.csv");
    std::string element;
    std::string element_lower;

    while(infile.good()) {
        getline(infile, element, ',');
        element_lower = element;
        std::transform(element_lower.begin(), element_lower.end(), element_lower.begin(), ::tolower);
        if (element_lower == a_name) {
            getline(infile, element, ',');
            this->type = element;
            getline(infile, element, ',');
            this->damage_mod = std::atof(element.c_str());
            getline(infile, element, ',');
            this->accuracy_mod = std::atof(element.c_str());
            getline(infile, element, ',');
            this->bleed_chance_mod = std::atof(element.c_str());
            getline(infile, element, ',');
            this->bleed_rounds = std::atoi(element.c_str());
            getline(infile, element, ',');
            this->bleed_damage = std::atoi(element.c_str());
            getline(infile, element, ',');
            this->stun_chance_mod = std::atof(element.c_str());
            getline(infile, element, ',');
            this->crit_chance_mod = std::atof(element.c_str());
            break;
        }
        else {
            getline(infile, element);
        }
    }
    infile.close();
}