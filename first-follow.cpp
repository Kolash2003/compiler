#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <string>

using namespace std;

class FirstFollowCalculator {
private:
    // Grammar representation
    map<char, vector<string>> productions;
    // Set of non-terminals
    set<char> non_terminals;
    // Set of terminals
    set<char> terminals;

    // Helper function to check if a symbol is a non-terminal
    bool is_non_terminal(char symbol) {
        return non_terminals.find(symbol) != non_terminals.end();
    }

    // Calculate FIRST set for a single symbol
    set<char> calculate_first_of_symbol(char symbol) {
        set<char> first_set;
        
        // If it's a terminal, FIRST is the symbol itself
        if (!is_non_terminal(symbol) && symbol != 'e') {
            first_set.insert(symbol);
            return first_set;
        }
        
        // If it's epsilon, add epsilon
        if (symbol == 'e') {
            first_set.insert('e');
            return first_set;
        }
        
        // For non-terminals, go through their productions
        for (const auto& production : productions[symbol]) {
            bool can_derive_epsilon = true;
            
            for (char first_symbol : production) {
                set<char> current_first = calculate_first_of_symbol(first_symbol);
                
                // Add all non-epsilon symbols from current symbol's FIRST
                for (char f : current_first) {
                    if (f != 'e') {
                        first_set.insert(f);
                    }
                }
                
                // If current symbol cannot derive epsilon, stop
                if (current_first.find('e') == current_first.end()) {
                    can_derive_epsilon = false;
                    break;
                }
            }
            
            // If entire production can derive epsilon, add epsilon to FIRST
            if (can_derive_epsilon) {
                first_set.insert('e');
            }
        }
        
        return first_set;
    }

    // Calculate FIRST set for a string of symbols
    set<char> calculate_first_of_string(const string& str) {
        set<char> first_set;
        bool can_derive_epsilon = true;
        
        for (char symbol : str) {
            set<char> current_first = calculate_first_of_symbol(symbol);
            
            // Add non-epsilon symbols
            for (char f : current_first) {
                if (f != 'e') {
                    first_set.insert(f);
                }
            }
            
            // If current symbol cannot derive epsilon, stop
            if (current_first.find('e') == current_first.end()) {
                can_derive_epsilon = false;
                break;
            }
        }
        
        // If entire string can derive epsilon, add epsilon
        if (can_derive_epsilon) {
            first_set.insert('e');
        }
        
        return first_set;
    }

    // Calculate FOLLOW sets
    map<char, set<char>> calculate_follow_sets() {
        map<char, set<char>> follow_sets;
        
        // Add $ (end of input) to start symbol's FOLLOW
        char start_symbol = productions.begin()->first;
        follow_sets[start_symbol].insert('$');
        
        bool changed;
        do {
            changed = false;
            
            // Iterate through all productions
            for (const auto& non_terminal : non_terminals) {
                for (const auto& production : productions[non_terminal]) {
                    for (size_t i = 0; i < production.length(); ++i) {
                        if (is_non_terminal(production[i])) {
                            // Case 1: A → αBβ, add FIRST(β) - ε to FOLLOW(B)
                            if (i + 1 < production.length()) {
                                set<char> first_of_next = calculate_first_of_symbol(production[i + 1]);
                                size_t original_size = follow_sets[production[i]].size();
                                
                                for (char f : first_of_next) {
                                    if (f != 'e') {
                                        follow_sets[production[i]].insert(f);
                                    }
                                }
                                
                                if (original_size != follow_sets[production[i]].size()) {
                                    changed = true;
                                }
                            }
                            
                            // Case 2: A → αB or A → αBβ where β can derive ε
                            // Add FOLLOW(A) to FOLLOW(B)
                            if (i + 1 == production.length() || 
                                calculate_first_of_symbol(production[i + 1]).find('e') != calculate_first_of_symbol(production[i + 1]).end()) {
                                size_t original_size = follow_sets[production[i]].size();
                                
                                for (char f : follow_sets[non_terminal]) {
                                    follow_sets[production[i]].insert(f);
                                }
                                
                                if (original_size != follow_sets[production[i]].size()) {
                                    changed = true;
                                }
                            }
                        }
                    }
                }
            }
        } while (changed);
        
        return follow_sets;
    }

public:
    // Add a production to the grammar
    void add_production(char non_terminal, const string& production) {
        productions[non_terminal].push_back(production);
        non_terminals.insert(non_terminal);
        
        // Identify terminals
        for (char symbol : production) {
            if (!is_non_terminal(symbol) && symbol != 'e') {
                terminals.insert(symbol);
            }
        }
    }

    // Calculate and print FIRST sets
    void print_first_sets() {
        cout << "FIRST SETS:\n";
        for (char non_terminal : non_terminals) {
            cout << "FIRST(" << non_terminal << ") = { ";
            set<char> first_set = calculate_first_of_symbol(non_terminal);
            for (char f : first_set) {
                cout << (f == 'e' ? "ε" : string(1, f)) << " ";
            }
            cout << "}\n";
        }
    }

    // Calculate and print FOLLOW sets
    void print_follow_sets() {
        map<char, set<char>> follow_sets = calculate_follow_sets();
        
        cout << "\nFOLLOW SETS:\n";
        for (char non_terminal : non_terminals) {
            cout << "FOLLOW(" << non_terminal << ") = { ";
            for (char f : follow_sets[non_terminal]) {
                cout << (f == 'e' ? "ε" : string(1, f)) << " ";
            }
            cout << "}\n";
        }
    }
};

int main() {
    FirstFollowCalculator calculator;

    // Example Grammar: Simple Expression Grammar
    // E → T + E | T
    // T → F * T | F
    // F → ( E ) | id

    calculator.add_production('E', "T+E");
    calculator.add_production('E', "T");
    calculator.add_production('T', "F*T");
    calculator.add_production('T', "F");
    calculator.add_production('F', "(E)");
    calculator.add_production('F', "id");

    // Calculate and print FIRST sets
    calculator.print_first_sets();

    // Calculate and print FOLLOW sets
    calculator.print_follow_sets();

    return 0;
}