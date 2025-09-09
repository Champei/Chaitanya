#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
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
    // Constructor
    Pokemon(string n, int hp, int atk, int def, int spd, vector<Move> m)
        : name(n), max_hp(hp), current_hp(hp),
          attack_stat(atk), defense_stat(def),
          speed(spd), moves(m) {}

    // Display stats
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

    // Check if fainted
    bool is_fainted() const {
        return current_hp <= 0;
    }

    // Take damage
    void take_damage(int dmg) {
        current_hp -= dmg;
        if (current_hp < 0) current_hp = 0;
        cout << name << " took " << dmg << " damage!" << endl;
    }

    // Attack another Pokémon
    void attack(Pokemon &other, int move_index) {
        if (move_index < 0 || move_index >= (int)moves.size()) {
            cout << "Invalid move index!" << endl;
            return;
        }

        Move move = moves[move_index];
        cout << name << " used " << move.name << "!" << endl;

        if (move.power == 0) {
            cout << "But it had no effect..." << endl;
            return;
        }

        // damage formula
        int damage = max(1, (attack_stat + move.power) - other.defense_stat);
        other.take_damage(damage);
    }
};

int main() {
    // Pikachu vs Charmander
    Pokemon pikachu("Pikachu", 100, 55, 40, 90, {
        {"Thunder Shock", 40}, {"Quick Attack", 30},
        {"Agility", 0}, {"Thunder", 70}
    });

    Pokemon charmander("Charmander", 90, 52, 43, 65, {
        {"Ember", 35}, {"Scratch", 25},
        {"Growl", 0}, {"Flamethrower", 60}
    });

    cout << "=== Example 1 ===" << endl;
    pikachu.display_stats();
    charmander.display_stats();

    pikachu.attack(charmander, 0); // Thunder Shock
    charmander.display_stats();

    cout << "Charmander fainted: "
         << (charmander.is_fainted() ? "True" : "False") << endl;
    cout << endl;

    // Magikarp vs Gyarados
    Pokemon magikarp("Magikarp", 30, 10, 55, 80, {
        {"Splash", 0}, {"Tackle", 20},
        {"Flail", 15}, {"Bounce", 25}
    });

    Pokemon gyarados("Gyarados", 150, 85, 79, 81, {
        {"Hydro Pump", 80}, {"Bite", 45},
        {"Dragon Rage", 50}, {"Hyper Beam", 90}
    });

    cout << "=== Example 2 ===" << endl;
    magikarp.display_stats();
    gyarados.display_stats();

    gyarados.attack(magikarp, 0); // Hydro Pump
    magikarp.display_stats();

    cout << "Magikarp fainted: "
         << (magikarp.is_fainted() ? "True" : "False") << endl;

    return 0;
}
