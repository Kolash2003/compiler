#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <sstream>

using namespace std;

map<char, set<string>> productions;  // Grammar rules
map<char, set<char>> firstSet, followSet;
set<char> nonTerminals, terminals;

// Function to compute FIRST set
void computeFirst(char symbol) {
    if (firstSet.count(symbol)) return;  // Already computed

    for (const string &prod : productions[symbol]) {
        bool nullable = true;  // Assume production is nullable initially
        for (char ch : prod) {
            if (ch == symbol) break;  // Prevent infinite recursion

            if (!isupper(ch)) {  // Terminal
                firstSet[symbol].insert(ch);
                nullable = false;
                break;
            } else {  // Non-terminal
                computeFirst(ch);
                firstSet[symbol].insert(firstSet[ch].begin(), firstSet[ch].end());
                if (firstSet[ch].find('ε') == firstSet[ch].end()) {
                    nullable = false;
                    break;
                }
            }
        }
        if (nullable) firstSet[symbol].insert('ε');
    }
}

// Function to compute FOLLOW set
void computeFollow(char symbol) {
    if (followSet.count(symbol)) return;  // Already computed

    if (symbol == *nonTerminals.begin()) {  // Start symbol
        followSet[symbol].insert('$');  // End-of-input marker
    }

    for (auto &prod : productions) {
        char lhs = prod.first;
        for (const string &rhs : prod.second) {
            size_t pos = rhs.find(symbol);
            while (pos != string::npos) {
                if (pos + 1 < rhs.size()) {  // Symbol is followed by something
                    char next = rhs[pos + 1];
                    if (!isupper(next)) {  // Terminal
                        followSet[symbol].insert(next);
                    } else {  // Non-terminal
                        computeFirst(next);
                        followSet[symbol].insert(firstSet[next].begin(), firstSet[next].end());
                        followSet[symbol].erase('ε');  // Remove epsilon if present

                        if (firstSet[next].count('ε')) {
                            computeFollow(lhs);
                            followSet[symbol].insert(followSet[lhs].begin(), followSet[lhs].end());
                        }
                    }
                } else {  // Symbol is at the end of the production
                    computeFollow(lhs);
                    followSet[symbol].insert(followSet[lhs].begin(), followSet[lhs].end());
                }
                pos = rhs.find(symbol, pos + 1);  // Find next occurrence
            }
        }
    }
}

// Driver function
int main() {
    int numProductions;
    cout << "Enter number of productions: ";
    cin >> numProductions;
    cin.ignore();

    cout << "Enter productions (use ε for epsilon):\n";
    for (int i = 0; i < numProductions; i++) {
        string line;
        getline(cin, line);
        char lhs = line[0];
        string rhs = line.substr(3);  // Remove "A -> "
        
        nonTerminals.insert(lhs);
        stringstream ss(rhs);
        string token;
        while (getline(ss, token, '|')) {
            productions[lhs].insert(token);
            for (char ch : token) {
                if (!isupper(ch) && ch != 'ε') terminals.insert(ch);
            }
        }
    }

    // Compute FIRST sets
    for (char nt : nonTerminals) {
        computeFirst(nt);
    }

    // Compute FOLLOW sets
    for (char nt : nonTerminals) {
        computeFollow(nt);
    }

    // Display FIRST sets
    cout << "\nFIRST Sets:\n";
    for (auto &entry : firstSet) {
        cout << "FIRST(" << entry.first << ") = { ";
        for (char ch : entry.second) cout << ch << " ";
        cout << "}\n";
    }

    // Display FOLLOW sets
    cout << "\nFOLLOW Sets:\n";
    for (auto &entry : followSet) {
        cout << "FOLLOW(" << entry.first << ") = { ";
        for (char ch : entry.second) cout << ch << " ";
        cout << "}\n";
    }

    return 0;
}
