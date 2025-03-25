#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>

using namespace std;

map<char, vector<string>> productions;
map<char, set<string>> first;
map<char, set<string>> follow;

// Function to compute FIRST set
void computeFirst(char nt) {
    if (!first[nt].empty()) return;  // Already computed

    for (const string &prod : productions[nt]) {
        char ch = prod[0];

        // If terminal, add directly
        if (!isupper(ch)) {
            first[nt].insert(string(1, ch));
        } 
        // If non-terminal, compute FIRST recursively
        else {
            computeFirst(ch);
            first[nt].insert(first[ch].begin(), first[ch].end());
        }
    }
}

// Function to compute FOLLOW set
void computeFollow(char nt) {
    if (!follow[nt].empty()) return;  // Already computed

    for (map<char, vector<string>>::iterator it = productions.begin(); it != productions.end(); ++it) {
        char lhs = it->first;
        vector<string> rhsList = it->second;

        for (const string &rhs : rhsList) {
            size_t pos = rhs.find(nt);
            if (pos != string::npos) {
                if (pos + 1 < rhs.size()) {  // If there is a next symbol
                    char next = rhs[pos + 1];
                    if (!isupper(next)) {
                        follow[nt].insert(string(1, next));
                    } else {
                        follow[nt].insert(first[next].begin(), first[next].end());
                    }
                } else {  // If at the end, add FOLLOW of LHS
                    computeFollow(lhs);
                    follow[nt].insert(follow[lhs].begin(), follow[lhs].end());
                }
            }
        }
    }
}

int main() {
    // Sample grammar input
    productions['E'] = {"TX"};
    productions['X'] = {"+TX", ""};  // Empty string represents epsilon
    productions['T'] = {"FY"};
    productions['Y'] = {"*FY", ""};
    productions['F'] = {"(E)", "id"};

    // Compute FIRST sets
    for (map<char, vector<string>>::iterator it = productions.begin(); it != productions.end(); ++it) {
        computeFirst(it->first);
    }

    // Compute FOLLOW sets (Assume 'E' is the start symbol)
    follow['E'].insert("$");
    for (map<char, vector<string>>::iterator it = productions.begin(); it != productions.end(); ++it) {
        computeFollow(it->first);
    }

    // Print FIRST sets
    cout << "FIRST sets:\n";
    for (map<char, set<string>>::iterator it = first.begin(); it != first.end(); ++it) {
        cout << "FIRST(" << it->first << ") = { ";
        for (set<string>::iterator sit = it->second.begin(); sit != it->second.end(); ++sit) {
            cout << *sit << " ";
        }
        cout << "}\n";
    }

    // Print FOLLOW sets
    cout << "\nFOLLOW sets:\n";
    for (map<char, set<string>>::iterator it = follow.begin(); it != follow.end(); ++it) {
        cout << "FOLLOW(" << it->first << ") = { ";
        for (set<string>::iterator sit = it->second.begin(); sit != it->second.end(); ++sit) {
            cout << *sit << " ";
        }
        cout << "}\n";
    }

    return 0;
}
