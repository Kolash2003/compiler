#include <iostream>
#include <map>

using namespace std;

// Structure to hold symbol table entry
struct Symbol {
    string name;    // Variable/Function name
    string type;    // Data type (int, float, char, etc.)
    int size;       // Size in bytes
    int scope;      // Scope level
};

// Class for Symbol Table
class SymbolTable {
    map<string, Symbol> table;  // Map to store symbols

public:
    // Insert a new symbol into the table
    void insert(string name, string type, int size, int scope) {
        if (table.count(name)) {
            cout << "Error: Symbol '" << name << "' already exists!\n";
            return;
        }
        table[name] = {name, type, size, scope};
        cout << "Inserted: " << name << " | Type: " << type << " | Size: " << size << " | Scope: " << scope << endl;
    }

    // Search for a symbol
    void search(string name) {
        if (table.count(name)) {
            Symbol s = table[name];
            cout << "Found: " << s.name << " | Type: " << s.type << " | Size: " << s.size << " | Scope: " << s.scope << endl;
        } else {
            cout << "Symbol '" << name << "' not found!\n";
        }
    }

    // Display the symbol table
    void display() {
        cout << "\nSymbol Table:\n";
        cout << "---------------------------------------------------\n";
        cout << "Name\t\tType\tSize\tScope\n";
        cout << "---------------------------------------------------\n";
        for (auto &entry : table) {
            Symbol s = entry.second;
            cout << s.name << "\t\t" << s.type << "\t" << s.size << "\t" << s.scope << endl;
        }
    }
};

// **Main Function**
int main() {
    SymbolTable symTable;
    
    // Insert some symbols
    symTable.insert("x", "int", 4, 1);
    symTable.insert("y", "float", 4, 1);
    symTable.insert("arr", "int[10]", 40, 1);
    symTable.insert("func", "void()", 0, 0);

    // Search for a symbol
    cout << "\nSearching for 'x'...\n";
    symTable.search("x");

    // Display symbol table
    symTable.display();

    return 0;
}
