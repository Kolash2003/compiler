#include <iostream>
#include <vector>
#include <stack>

using namespace std;

// Structure to store a quadruple representation
struct Quadruple {
    string op, arg1, arg2, result;
};

// Class to generate Quadruples
class QuadrupleGenerator {
    vector<Quadruple> quadruples;
    stack<string> operands;  // Stack to hold operands
    stack<char> operators;   // Stack to hold operators
    int tempCount = 1;       // Temporary variable counter

    // Generate a new temporary variable (t1, t2, t3...)
    string newTemp() {
        return "t" + to_string(tempCount++);
    }

public:
    // Function to generate quadruples for an arithmetic expression
    void generate(string expr) {
        for (char ch : expr) {
            if (isdigit(ch) || isalpha(ch)) {
                operands.push(string(1, ch));
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

        string right = operands.top(); operands.pop();
        string left = operands.top(); operands.pop();
        char op = operators.top(); operators.pop();

        string temp = newTemp();
        quadruples.push_back({string(1, op), left, right, temp});
        operands.push(temp);
    }

    // Function to display the generated quadruples
    void display() {
        cout << "\nQuadruples Representation:\n";
        cout << "----------------------------------\n";
        cout << "OP\tARG1\tARG2\tRESULT\n";
        cout << "----------------------------------\n";
        for (const auto &q : quadruples) {
            cout << q.op << "\t" << q.arg1 << "\t" << q.arg2 << "\t" << q.result << endl;
        }
    }
};

// **Main Function**
int main() {
    string expression;
    cout << "Enter arithmetic expression (e.g., a+b*c): ";
    cin >> expression;

    QuadrupleGenerator generator;
    generator.generate(expression);
    generator.display();

    return 0;
}
