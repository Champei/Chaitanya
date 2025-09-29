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
    string effect; 
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

    int poisonTurns;
    int paralysisTurns;
    int sleepTurns;

public:
    Pokemon(string n, int hp, int atk, int def, int spd, vector<Move> m)
        : name(n), max_hp(hp), current_hp(hp),
          attack_stat(atk), defense_stat(def), speed(spd), moves(m),
          poisonTurns(0), paralysisTurns(0), sleepTurns(0) {}

    string get_name() const { return name; }
    int get_hp() const { return current_hp; }
    int get_max_hp() const { return max_hp; }
    int get_attack() const { return attack_stat; }
    int get_defense() const { return defense_stat; }
    int get_speed() const { return speed; }
    vector<Move> get_moves() const { return moves; }

    bool is_poisoned() const { return poisonTurns > 0; }
    bool is_paralyzed() const { return paralysisTurns > 0; }
    bool is_asleep() const { return sleepTurns > 0; }

    void apply_status(string effect, int duration) {
        if (effect == "poison") poisonTurns = duration;
        if (effect == "paralysis") paralysisTurns = duration;
        if (effect == "sleep") sleepTurns = duration;
    }

    void update_status_end_turn() {
        if (poisonTurns > 0) {
            int dmg = max(1, max_hp / 10); 
            take_damage(dmg);
            poisonTurns--;
            cout << name << " is hurt by poison! (" << poisonTurns << " turns left)\n";
        }
        if (paralysisTurns > 0) {
            paralysisTurns--;
        }
        if (sleepTurns > 0) {
            sleepTurns--;
        }
    }

    bool can_act(mt19937 &rng) {
        if (sleepTurns > 0) {
            cout << name << " is asleep and cannot move!\n";
            return false;
        }
        if (paralysisTurns > 0) {
            uniform_int_distribution<int> dist(0, 1);
            if (dist(rng) == 0) {
                cout << name << " is paralyzed and cannot move!\n";
                return false;
            }
        }
        return true;
    }

    bool is_fainted() const { return current_hp <= 0; }

    void take_damage(int dmg) {
        current_hp -= dmg;
        if (current_hp < 0) current_hp = 0;
        cout << name << " took " << dmg << " damage!" << endl;
    }

    void display_stats() const {
        cout << name << " - HP: " << current_hp << "/" << max_hp
             << ", Atk: " << attack_stat
             << ", Def: " << defense_stat
             << ", Spd: " << speed << endl;
    }
};

class Battle {
protected:
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
        double crit_mult = critical ? 2.0 : 1.0;
        double base = (double)(attacker.get_attack() * move.power) / defender.get_defense();
        return max(1, (int)(base * crit_mult));
    }
};

class AIBattle : public Battle {
private:
    string difficulty;

public:
    AIBattle(Pokemon &a, Pokemon &b, string diff) : Battle(a, b), difficulty(diff) {}

    int choose_ai_move(Pokemon &ai, Pokemon &opponent) {
        vector<Move> moves = ai.get_moves();

        if (difficulty == "hard") {
            if (opponent.get_hp() > opponent.get_max_hp() * 0.7) {
                for (size_t i = 0; i < moves.size(); i++) {
                    if (moves[i].effect != "") return (int)i;
                }
            }
            if (opponent.get_hp() < opponent.get_max_hp() * 0.25) {
                int best = 0;
                for (size_t i = 1; i < moves.size(); i++) {
                    if (moves[i].power > moves[best].power) best = i;
                }
                return best;
            }
        }
        if (difficulty == "medium") {
            int best = 0;
            for (size_t i = 1; i < moves.size(); i++) {
                if (moves[i].power > moves[best].power) best = i;
            }
            return best;
        }
        return random_move_index(ai);
    }

    void start_battle() {
        cout << "=== AI POKÉMON BATTLE BEGINS! ===" << endl;
        cout << p1.get_name() << " (HP: " << p1.get_hp() << ") VS "
             << p2.get_name() << " (HP: " << p2.get_hp() << ") [COMPUTER]\n\n";

        while (!p1.is_fainted() && !p2.is_fainted()) {
            turn_count++;
            Pokemon *first, *second;
            if (p1.get_speed() > p2.get_speed()) { first = &p1; second = &p2; }
            else if (p2.get_speed() > p1.get_speed()) { first = &p2; second = &p1; }
            else { first = chance(0.5) ? &p1 : &p2; second = (first == &p1 ? &p2 : &p1); }

            if (!first->is_fainted() && first->can_act(rng)) {
                int move_index = (first == &p2) ? choose_ai_move(*first, *second) : random_move_index(*first);
                Move move = first->get_moves()[move_index];
                cout << first->get_name() << " used " << move.name << "!" << endl;

                if (move.effect != "" && move.power == 0) {
                    cout << second->get_name() << " is now " << move.effect << "!" << endl;
                    if (move.effect == "poison") second->apply_status("poison", 4);
                    if (move.effect == "paralysis") second->apply_status("paralysis", 3);
                    if (move.effect == "sleep") second->apply_status("sleep", 2 + rand() % 3);
                } else {
                    bool critical;
                    int dmg = calculate_damage(*first, *second, move, critical);
                    if (critical) { cout << "Critical Hit!\n"; crit_hits++; }
                    second->take_damage(dmg);
                }
                cout << second->get_name() << " HP: " << second->get_hp() << "/" << second->get_max_hp() << "\n\n";
            }

            if (second->is_fainted()) break;

            if (!second->is_fainted() && second->can_act(rng)) {
                int move_index = (second == &p2) ? choose_ai_move(*second, *first) : random_move_index(*second);
                Move move = second->get_moves()[move_index];
                cout << second->get_name() << " used " << move.name << "!" << endl;

                if (move.effect != "" && move.power == 0) {
                    cout << first->get_name() << " is now " << move.effect << "!" << endl;
                    if (move.effect == "poison") first->apply_status("poison", 4);
                    if (move.effect == "paralysis") first->apply_status("paralysis", 3);
                    if (move.effect == "sleep") first->apply_status("sleep", 2 + rand() % 3);
                } else {
                    bool critical;
                    int dmg = calculate_damage(*second, *first, move, critical);
                    if (critical) { cout << "Critical Hit!\n"; crit_hits++; }
                    first->take_damage(dmg);
                }
                cout << first->get_name() << " HP: " << first->get_hp() << "/" << first->get_max_hp() << "\n\n";
            }

            p1.update_status_end_turn();
            p2.update_status_end_turn();
        }

        Pokemon *winner = p1.is_fainted() ? &p2 : &p1;
        Pokemon *loser = p1.is_fainted() ? &p1 : &p2;

        cout << loser->get_name() << " fainted!\n";
        cout << "🏆 " << winner->get_name() << " wins the battle!\n";
    }
};

int main() {
    Pokemon venomoth("Venomoth", 95, 65, 60, 90, {
        {"Poison Powder", 0, "poison"},
        {"Psychic", 55, ""},
        {"Sleep Powder", 0, "sleep"},
        {"Bug Buzz", 50, ""}
    });

    Pokemon alakazam("Alakazam", 85, 90, 45, 120, {
        {"Psychic", 55, ""},
        {"Thunder Wave", 0, "paralysis"},
        {"Teleport", 0, ""},
        {"Psybeam", 45, ""}
    });

    AIBattle ai_battle(venomoth, alakazam, "hard");
    ai_battle.start_battle();

    return 0;
}
