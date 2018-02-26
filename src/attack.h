#ifndef ATTACK_H
#define ATTACK_H


class Attack {
    public:
        Attack();
        Attack(std::string n);
        void parseCSV(std::string n);
        std::string name;
        std::string type;

    private:
        double accuracy_mod;
        double damage_mod;
        double stun_chance_mod;
        double crit_chance_mod;
        double bleed_chance_mod;
        int bleed_rounds;
        int bleed_damage;
};

#endif