#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <queue>
#include <iomanip>

class NFAToDFAConverter {
private:
    // NFA transition function
    std::map<std::pair<int, char>, std::set<int>> nfa_transitions;
    
    // Set of NFA states
    std::set<int> nfa_states;
    
    // Set of NFA final states
    std::set<int> nfa_final_states;
    
    // Alphabet symbols
    std::set<char> alphabet;
    
    // Start state
    int nfa_start_state;

    // Epsilon closure for a single state
    std::set<int> epsilon_closure(int state) {
        std::set<int> closure;
        std::vector<int> stack = {state};
        
        while (!stack.empty()) {
            int current = stack.back();
            stack.pop_back();
            
            // If state already in closure, skip
            if (closure.find(current) != closure.end()) continue;
            
            closure.insert(current);
            
            // Find epsilon transitions (assuming 'ε' represents epsilon)
            auto epsilon_trans = nfa_transitions.find({current, 'ε'});
            if (epsilon_trans != nfa_transitions.end()) {
                for (int next_state : epsilon_trans->second) {
                    if (closure.find(next_state) == closure.end()) {
                        stack.push_back(next_state);
                    }
                }
            }
        }
        
        return closure;
    }

    // Epsilon closure for a set of states
    std::set<int> epsilon_closure(const std::set<int>& states) {
        std::set<int> closure;
        for (int state : states) {
            std::set<int> state_closure = epsilon_closure(state);
            closure.insert(state_closure.begin(), state_closure.end());
        }
        return closure;
    }

    // Move function for a set of states on a given symbol
    std::set<int> move(const std::set<int>& states, char symbol) {
        std::set<int> moved_states;
        for (int state : states) {
            auto trans = nfa_transitions.find({state, symbol});
            if (trans != nfa_transitions.end()) {
                moved_states.insert(trans->second.begin(), trans->second.end());
            }
        }
        return moved_states;
    }

public:
    // Add NFA transition
    void add_transition(int from_state, char symbol, int to_state) {
        nfa_transitions[{from_state, symbol}].insert(to_state);
        nfa_states.insert(from_state);
        nfa_states.insert(to_state);
        alphabet.insert(symbol);
    }

    // Set NFA start state
    void set_start_state(int start_state) {
        nfa_start_state = start_state;
        nfa_states.insert(start_state);
    }

    // Add NFA final state
    void add_final_state(int final_state) {
        nfa_final_states.insert(final_state);
    }

    // Convert NFA to DFA
    std::map<std::pair<int, char>, int> convert_to_dfa() {
        std::map<std::pair<int, char>, int> dfa_transitions;
        std::map<std::set<int>, int> dfa_state_map;
        std::queue<std::set<int>> unmarked_states;
        int dfa_state_counter = 0;

        // Start with epsilon closure of start state
        std::set<int> start_state_closure = epsilon_closure(nfa_start_state);
        dfa_state_map[start_state_closure] = dfa_state_counter++;
        unmarked_states.push(start_state_closure);

        while (!unmarked_states.empty()) {
            std::set<int> current_nfa_states = unmarked_states.front();
            unmarked_states.pop();
            int current_dfa_state = dfa_state_map[current_nfa_states];

            // For each symbol in alphabet
            for (char symbol : alphabet) {
                if (symbol == 'ε') continue;  // Skip epsilon
                
                // Move and get epsilon closure
                std::set<int> next_nfa_states = epsilon_closure(move(current_nfa_states, symbol));
                
                // If this is a new DFA state
                if (!next_nfa_states.empty() && 
                    dfa_state_map.find(next_nfa_states) == dfa_state_map.end()) {
                    dfa_state_map[next_nfa_states] = dfa_state_counter++;
                    unmarked_states.push(next_nfa_states);
                }

                // If we found a valid transition
                if (!next_nfa_states.empty()) {
                    dfa_transitions[{current_dfa_state, symbol}] = 
                        dfa_state_map[next_nfa_states];
                }
            }
        }

        // Print DFA details
        print_dfa_details(dfa_state_map, dfa_transitions);

        return dfa_transitions;
    }

    // Print DFA details
    void print_dfa_details(const std::map<std::set<int>, int>& dfa_state_map, 
                            const std::map<std::pair<int, char>, int>& dfa_transitions) {
        std::cout << "\nDFA State Mapping:\n";
        for (const auto& state_map : dfa_state_map) {
            std::cout << "DFA State " << dfa_state_map.at(state_map.first) << " = {";
            for (int nfa_state : state_map.first) {
                std::cout << nfa_state << " ";
            }
            std::cout << "}\n";
        }

        std::cout << "\nDFA Transitions:\n";
        for (const auto& trans : dfa_transitions) {
            std::cout << "δ(q" << trans.first.first << ", " 
                      << trans.first.second << ") = q" 
                      << trans.second << std::endl;
        }
    }
};

int main() {
    NFAToDFAConverter converter;

    // Example NFA
    // States: 0 (start), 1, 2 (final)
    // Transitions: 
    // 0 --a--> 0, 1
    // 0 --b--> 0
    // 0 --ε--> 1
    // 1 --a--> 2

    // Set start state
    converter.set_start_state(0);

    // Add transitions
    converter.add_transition(0, 'a', 0);
    converter.add_transition(0, 'a', 1);
    converter.add_transition(0, 'b', 0);
    converter.add_transition(0, 'ε', 1);
    converter.add_transition(1, 'a', 2);

    // Add final states
    converter.add_final_state(2);

    // Convert NFA to DFA
    converter.convert_to_dfa();

    return 0;
}