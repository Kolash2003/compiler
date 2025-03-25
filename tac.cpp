#include <iostream>
#include <vector>
#include <stack>

using namespace std;

// Structure to represent a Three Address Code instruction
struct TAC {
    string result, op, arg1, arg2;
};

// Class to generate TAC
class TACGenerator {
    vector<TAC> instructions;
    stack<string> tempVars;
    int tempCount = 1;  // Counter for temporary variables

    // Generate a new temporary variable (t1, t2, t3...)
    string newTemp() {
        return "t" + to_string(tempCount++);
    }

public:
    // Function to generate TAC for an expression
    string generate(string expr) {
        stack<char> operators;
        stack<string> operands;

        for (char ch : expr) {
            if (isdigit(ch) || isalpha(ch)) {
                operands.push(string(1, ch));
            } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
                while (!operators.empty() && precedence(operators.top()) >= precedence(ch)) {
                    process(operators, operands);
                }
                operators.push(ch);
            }
        }

        while (!operators.empty()) {
            process(operators, operands);
        }

        return operands.top();
    }

    // Function to return operator precedence
    int precedence(char op) {
        if (op == '*' || op == '/') return 2;
        if (op == '+' || op == '-') return 1;
        return 0;
    }

    // Process an operator from the stack
    void process(stack<char> &operators, stack<string> &operands) {
        if (operands.size() < 2) return;

        string right = operands.top(); operands.pop();
        string left = operands.top(); operands.pop();
        char op = operators.top(); operators.pop();

        string temp = newTemp();
        instructions.push_back({temp, string(1, op), left, right});
        operands.push(temp);
    }

    // Function to display the generated TAC
    void display() {
        cout << "\nThree Address Code (TAC):\n";
        for (const auto &inst : instructions) {
            cout << inst.result << " = " << inst.arg1 << " " << inst.op << " " << inst.arg2 << endl;
        }
    }
};

// **Main Function**
int main() {
    string expression;
    cout << "Enter arithmetic expression (e.g., a+b*c): ";
    cin >> expression;

    TACGenerator generator;
    generator.generate(expression);
    generator.display();

    return 0;
}
