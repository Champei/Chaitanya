#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
using namespace std;

struct Move {
    string name;
    int power;
};

class Pokemon {
private:
    string name;
    int max_hp;
    int current_hp;
    int attack_stat;
    int defense_stat;
    int speed;
    vector<Move> moves;

public:
    Pokemon(string n, int hp, int atk, int def, int spd, vector<Move> m)
        : name(n), max_hp(hp), current_hp(hp),
          attack_stat(atk), defense_stat(def),
          speed(spd), moves(m) {}

    string get_name() const { return name; }
    int get_hp() const { return current_hp; }
    int get_max_hp() const { return max_hp; }
    int get_attack() const { return attack_stat; }
    int get_defense() const { return defense_stat; }
    int get_speed() const { return speed; }
    vector<Move> get_moves() const { return moves; }

    bool is_fainted() const { return current_hp <= 0; }

    void take_damage(int dmg) {
        current_hp -= dmg;
        if (current_hp < 0) current_hp = 0;
        cout << name << " took " << dmg << " damage!" << endl;
    }

    void display_stats() const {
        cout << name << " - HP: " << current_hp << "/" << max_hp
             << ", Attack: " << attack_stat
             << ", Defense: " << defense_stat
             << ", Speed: " << speed << endl;
        cout << "Moves: ";
        for (size_t i = 0; i < moves.size(); i++) {
            cout << moves[i].name << " (" << moves[i].power << ")";
            if (i < moves.size() - 1) cout << ", ";
        }
        cout << "\n" << endl;
    }
};


class Battle {
private:
    Pokemon &p1;
    Pokemon &p2;
    int turn_count;
    int crit_hits;

    mt19937 rng;

public:
    Battle(Pokemon &a, Pokemon &b) : p1(a), p2(b), turn_count(0), crit_hits(0) {
        rng.seed((unsigned)time(0));
    }

    bool chance(double probability) {
        uniform_real_distribution<double> dist(0.0, 1.0);
        return dist(rng) < probability;
    }

    int random_move_index(const Pokemon &p) {
        uniform_int_distribution<int> dist(0, (int)p.get_moves().size() - 1);
        return dist(rng);
    }

    int calculate_damage(const Pokemon &attacker, const Pokemon &defender,
                         const Move &move, bool &critical) {
        if (move.power == 0) return 0;

        critical = chance(0.10); 
        double crit_multiplier = critical ? 2.0 : 1.0;

        double base = (double)(attacker.get_attack() * move.power) / defender.get_defense();
        int damage = max(1, (int)(base * crit_multiplier));
        return damage;
    }

    void start_battle(bool auto_moves = true) {
        cout << "=== POKÉMON BATTLE BEGINS! ===" << endl;
        cout << p1.get_name() << " (HP: " << p1.get_hp() << "/" << p1.get_max_hp() << ") VS "
             << p2.get_name() << " (HP: " << p2.get_hp() << "/" << p2.get_max_hp() << ")\n"
             << endl;

        while (!p1.is_fainted() && !p2.is_fainted()) {
            turn_count++;

            Pokemon *first;
            Pokemon *second;
            if (p1.get_speed() > p2.get_speed()) {
                first = &p1; second = &p2;
                cout << "Turn " << turn_count << ": " << p1.get_name()
                     << " goes first! (Speed: " << p1.get_speed()
                     << " vs " << p2.get_speed() << ")" << endl;
            } else if (p2.get_speed() > p1.get_speed()) {
                first = &p2; second = &p1;
                cout << "Turn " << turn_count << ": " << p2.get_name()
                     << " goes first! (Speed: " << p2.get_speed()
                     << " vs " << p1.get_speed() << ")" << endl;
            } else {
                // Speed tie → random
                if (chance(0.5)) { first = &p1; second = &p2; }
                else { first = &p2; second = &p1; }
                cout << "Turn " << turn_count << ": Speed tie! "
                     << first->get_name() << " goes first! (Speed: "
                     << p1.get_speed() << " vs " << p2.get_speed() << ")" << endl;
            }

            if (!first->is_fainted()) {
                int move_index = auto_moves ? random_move_index(*first) : 0;
                Move move = first->get_moves()[move_index];
                cout << first->get_name() << " used " << move.name << "!" << endl;

                bool critical;
                int damage = calculate_damage(*first, *second, move, critical);
                if (move.power == 0) {
                    cout << "But it had no effect..." << endl;
                } else {
                    if (critical) {
                        cout << "Critical Hit!" << endl;
                        crit_hits++;
                    }
                    second->take_damage(damage);
                }
                cout << second->get_name() << " HP: "
                     << second->get_hp() << "/" << second->get_max_hp() << "\n"
                     << endl;
            }

            if (second->is_fainted()) break;

            if (!second->is_fainted()) {
                int move_index = auto_moves ? random_move_index(*second) : 0;
                Move move = second->get_moves()[move_index];
                cout << second->get_name() << " used " << move.name << "!" << endl;

                bool critical;
                int damage = calculate_damage(*second, *first, move, critical);
                if (move.power == 0) {
                    cout << "But it had no effect..." << endl;
                } else {
                    if (critical) {
                        cout << "Critical Hit!" << endl;
                        crit_hits++;
                    }
                    first->take_damage(damage);
                }
                cout << first->get_name() << " HP: "
                     << first->get_hp() << "/" << first->get_max_hp() << "\n"
                     << endl;
            }
        }

        Pokemon *winner = p1.is_fainted() ? &p2 : &p1;
        Pokemon *loser = p1.is_fainted() ? &p1 : &p2;

        cout << loser->get_name() << " fainted!" << endl;
        cout << "🏆 " << winner->get_name() << " wins the battle!" << endl;

        cout << "\nBattle Summary:" << endl;
        cout << "- Winner: " << winner->get_name() << endl;
        cout << "- Turns: " << turn_count << endl;
        cout << "- Critical Hits: " << crit_hits << endl;
    }
};

// Example Run

int main() {
    Pokemon pikachu("Pikachu", 100, 55, 40, 90, {
        {"Thunder Shock", 40}, {"Quick Attack", 30},
        {"Agility", 0}, {"Thunder", 70}
    });

    Pokemon charmander("Charmander", 90, 52, 43, 65, {
        {"Ember", 35}, {"Scratch", 25},
        {"Growl", 0}, {"Flamethrower", 60}
    });

    Battle battle(pikachu, charmander);
    battle.start_battle(true); 
    return 0;
}
