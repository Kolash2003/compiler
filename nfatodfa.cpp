#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <queue>

using namespace std;

typedef map<int, map<char, set<int>>> NFA;  // NFA: state → (symbol → set of next states)
typedef map<int, map<char, int>> DFA;       // DFA: state → (symbol → next state)

// Function to compute epsilon closure of a state
set<int> epsilonClosure(int state, NFA &nfa) {
    set<int> closure;
    queue<int> q;
    
    closure.insert(state);
    q.push(state);

    while (!q.empty()) {
        int curr = q.front();
        q.pop();

        if (nfa[curr].count('ε')) {  // Epsilon transition exists
            for (int next : nfa[curr]['ε']) {
                if (closure.find(next) == closure.end()) {
                    closure.insert(next);
                    q.push(next);
                }
            }
        }
    }
    return closure;
}

// Convert NFA to DFA
void convertNFAtoDFA(NFA &nfa) {
    map<set<int>, int> stateMapping;  // Mapping of state sets to DFA states
    DFA dfaTransitions;
    queue<set<int>> stateQueue;
    
    set<int> startClosure = epsilonClosure(0, nfa);
    stateMapping[startClosure] = 0;
    stateQueue.push(startClosure);
    
    int dfaStateCount = 0;

    while (!stateQueue.empty()) {
        set<int> currentSet = stateQueue.front();
        stateQueue.pop();
        int currentState = stateMapping[currentSet];

        map<char, set<int>> newTransitions;

        for (int state : currentSet) {
            for (auto it = nfa[state].begin(); it != nfa[state].end(); ++it) {
                char symbol = it->first;
                if (symbol == 'ε') continue;  // Skip epsilon transitions
                
                for (int next : it->second) {
                    newTransitions[symbol].insert(next);
                }
            }
        }

        for (auto it = newTransitions.begin(); it != newTransitions.end(); ++it) {
            char symbol = it->first;
            set<int> nextStateSet;

            for (int nextState : it->second) {
                set<int> closure = epsilonClosure(nextState, nfa);
                nextStateSet.insert(closure.begin(), closure.end());
            }

            if (stateMapping.find(nextStateSet) == stateMapping.end()) {
                dfaStateCount++;
                stateMapping[nextStateSet] = dfaStateCount;
                stateQueue.push(nextStateSet);
            }
            dfaTransitions[currentState][symbol] = stateMapping[nextStateSet];
        }
    }

    // Print DFA transitions
    cout << "DFA Transitions:\n";
    for (auto it = dfaTransitions.begin(); it != dfaTransitions.end(); ++it) {
        int state = it->first;
        for (auto transIt = it->second.begin(); transIt != it->second.end(); ++transIt) {
            cout << "State " << state << " -- " << transIt->first << " --> " << transIt->second << endl;
        }
    }
}

// Driver Code
int main() {
    NFA nfa;
    
    // Example NFA
    nfa[0]['a'] = {0, 1};  // State 0 → 'a' → {0,1}
    nfa[0]['b'] = {0};     // State 0 → 'b' → {0}
    nfa[0]['ε'] = {2};     // State 0 → ε → {2}
    nfa[1]['b'] = {3};     // State 1 → 'b' → {3}
    nfa[2]['a'] = {3};     // State 2 → 'a' → {3}
    nfa[3]['b'] = {3};     // State 3 → 'b' → {3}

    convertNFAtoDFA(nfa);

    return 0;
}
