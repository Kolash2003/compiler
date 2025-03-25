#include <iostream>
#include <vector>
#include <stack>

using namespace std;

// Structure to store a triple representation
struct Triple {
    string op, arg1, arg2;
};

// Class to generate Triples
class TripleGenerator {
    vector<Triple> triples;
    stack<int> operands;  // Stack to hold operand indices
    stack<char> operators;   // Stack to hold operators

public:
    // Function to generate triples for an arithmetic expression
    void generate(string expr) {
        for (char ch : expr) {
            if (isalnum(ch)) { // If operand (variable or digit)
                operands.push(triples.size());  // Push its index
                triples.push_back({"", string(1, ch), ""});
            } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
                while (!operators.empty() && precedence(operators.top()) >= precedence(ch)) {
                    process();
                }
                operators.push(ch);
            }
        }

        while (!operators.empty()) {
            process();
        }
    }

    // Function to return operator precedence
    int precedence(char op) {
        if (op == '*' || op == '/') return 2;
        if (op == '+' || op == '-') return 1;
        return 0;
    }

    // Process an operator from the stack
    void process() {
        if (operands.size() < 2) return;

        int right = operands.top(); operands.pop();
        int left = operands.top(); operands.pop();
        char op = operators.top(); operators.pop();

        triples.push_back({string(1, op), to_string(left), to_string(right)});
        operands.push(triples.size() - 1);  // Push the index of the result
    }

    // Function to display the generated triples
    void display() {
        cout << "\nTriples Representation:\n";
        cout << "----------------------------------\n";
        cout << "INDEX\tOP\tARG1\tARG2\n";
        cout << "----------------------------------\n";
        for (size_t i = 0; i < triples.size(); i++) {
            cout << i << "\t" << triples[i].op << "\t" << triples[i].arg1 << "\t" << triples[i].arg2 << endl;
        }
    }
};

// **Main Function**
int main() {
    string expression;
    cout << "Enter arithmetic expression (e.g., a+b*c): ";
    cin >> expression;

    TripleGenerator generator;
    generator.generate(expression);
    generator.display();

    return 0;
}
