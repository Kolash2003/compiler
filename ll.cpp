#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>

using namespace std;

map<char, set<char>> first, follow;
map<char, vector<string>> productions;
map<pair<char, char>, string> parsingTable;
set<char> terminals, nonTerminals;

void computeFirst(char symbol);
void computeFollow(char symbol);
void constructLL1Table();
void displayParsingTable();

int main() {
    int n;
    cout << "Enter number of productions: ";
    cin >> n;

    cout << "Enter productions (e.g., E->TX, X->+TX|ε):\n";
    for (int i = 0; i < n; i++) {
        string prod;
        cin >> prod;
        char nonTerm = prod[0];
        nonTerminals.insert(nonTerm);
        string rhs = prod.substr(3);
        size_t pos = 0;
        while ((pos = rhs.find('|')) != string::npos) {
            productions[nonTerm].push_back(rhs.substr(0, pos));
            rhs.erase(0, pos + 1);
        }
        productions[nonTerm].push_back(rhs);
    }

    for (map<char, vector<string>>::iterator it = productions.begin(); it != productions.end(); ++it) {
        computeFirst(it->first);
    }

    for (map<char, vector<string>>::iterator it = productions.begin(); it != productions.end(); ++it) {
        computeFollow(it->first);
    }

    constructLL1Table();
    displayParsingTable();
    
    return 0;
}

// Compute FIRST set for a given non-terminal
void computeFirst(char symbol) {
    if (!first[symbol].empty()) return;  // Already computed

    for (vector<string>::iterator it = productions[symbol].begin(); it != productions[symbol].end(); ++it) {
        string prod = *it;
        for (size_t i = 0; i < prod.size(); i++) {
            char ch = prod[i];

            if (!isupper(ch)) {  // Terminal
                first[symbol].insert(ch);
                terminals.insert(ch);
                break;
            } else {  // Non-terminal
                computeFirst(ch);
                first[symbol].insert(first[ch].begin(), first[ch].end());
                if (first[ch].find('ε') == first[ch].end()) break;  // Stop if ε is not in FIRST(ch)
            }
        }
    }
}

// Compute FOLLOW set for a given non-terminal
void computeFollow(char symbol) {
    if (!follow[symbol].empty()) return;  // Already computed

    if (symbol == *nonTerminals.begin()) follow[symbol].insert('$'); // Start symbol

    for (map<char, vector<string>>::iterator it = productions.begin(); it != productions.end(); ++it) {
        char nt = it->first;
        vector<string> prods = it->second;

        for (vector<string>::iterator pIt = prods.begin(); pIt != prods.end(); ++pIt) {
            string prod = *pIt;
            size_t pos = prod.find(symbol);
            while (pos != string::npos) {
                if (pos + 1 < prod.length()) {  // Symbol is not at the end
                    char next = prod[pos + 1];
                    if (!isupper(next)) follow[symbol].insert(next);
                    else {
                        follow[symbol].insert(first[next].begin(), first[next].end());
                        follow[symbol].erase('ε');  // Remove ε if present
                        if (first[next].find('ε') != first[next].end()) computeFollow(nt);
                    }
                } else {  // Symbol at end, inherit FOLLOW of nt
                    computeFollow(nt);
                    follow[symbol].insert(follow[nt].begin(), follow[nt].end());
                }
                pos = prod.find(symbol, pos + 1);
            }
        }
    }
}

// Construct the LL(1) Parsing Table
void constructLL1Table() {
    for (map<char, vector<string>>::iterator it = productions.begin(); it != productions.end(); ++it) {
        char nt = it->first;
        vector<string> prods = it->second;

        for (vector<string>::iterator pIt = prods.begin(); pIt != prods.end(); ++pIt) {
            string prod = *pIt;
            set<char> firstSet;
            
            for (size_t i = 0; i < prod.size(); i++) {
                char ch = prod[i];

                if (!isupper(ch)) {
                    firstSet.insert(ch);
                    break;
                } else {
                    firstSet.insert(first[ch].begin(), first[ch].end());
                    if (first[ch].find('ε') == first[ch].end()) break;
                }
            }

            for (set<char>::iterator fIt = firstSet.begin(); fIt != firstSet.end(); ++fIt) {
                if (*fIt != 'ε') parsingTable[{nt, *fIt}] = prod;
            }

            if (firstSet.find('ε') != firstSet.end()) {
                for (set<char>::iterator fIt = follow[nt].begin(); fIt != follow[nt].end(); ++fIt) {
                    parsingTable[{nt, *fIt}] = "ε";
                }
            }
        }
    }
}

// Display the LL(1) Parsing Table
void displayParsingTable() {
    cout << "\nLL(1) Parsing Table:\n";
    cout << "------------------------------------\n";
    cout << "   | ";
    
    for (set<char>::iterator it = terminals.begin(); it != terminals.end(); ++it) {
        cout << *it << "  | ";
    }
    
    cout << "$  |\n";
    cout << "------------------------------------\n";

    for (set<char>::iterator it = nonTerminals.begin(); it != nonTerminals.end(); ++it) {
        char nt = *it;
        cout << nt << "  | ";

        for (set<char>::iterator termIt = terminals.begin(); termIt != terminals.end(); ++termIt) {
            char terminal = *termIt;
            if (parsingTable.find({nt, terminal}) != parsingTable.end())
                cout << nt << "->" << parsingTable[{nt, terminal}] << " | ";
            else 
                cout << "  -  | ";
        }

        if (parsingTable.find({nt, '$'}) != parsingTable.end())
            cout << nt << "->" << parsingTable[{nt, '$'}] << " |\n";
        else 
            cout << "  -  |\n";
    }
    
    cout << "------------------------------------\n";
}
