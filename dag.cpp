#include <iostream>
#include <map>
#include <vector>
#include <set>

using namespace std;

// Structure for a DAG node
struct Node {
    string value;  // Operator or operand
    Node *left, *right;  // Children nodes

    Node(string val) : value(val), left(nullptr), right(nullptr) {}
};

// Class to construct and manage the DAG
class DAG {
    map<string, Node*> nodeMap;  // Store already created nodes (for common subexpression elimination)

public:
    Node* createNode(string value, Node* left = nullptr, Node* right = nullptr) {
        string key = value;
        if (left) key += left->value;
        if (right) key += right->value;

        // If node already exists (common subexpression elimination)
        if (nodeMap.count(key)) return nodeMap[key];

        // Create new node
        Node* newNode = new Node(value);
        newNode->left = left;
        newNode->right = right;
        nodeMap[key] = newNode;
        return newNode;
    }

    // Print DAG in inorder traversal
    void inorder(Node* root) {
        if (!root) return;
        if (root->left) cout << "(";
        inorder(root->left);
        cout << root->value;
        inorder(root->right);
        if (root->right) cout << ")";
    }
};

// Function to build a DAG from a given expression
Node* constructDAG(string expr, DAG &dag) {
    vector<Node*> operandStack;
    vector<char> operatorStack;
    set<char> operators = {'+', '-', '*', '/'};

    for (size_t i = 0; i < expr.size(); i++) {
        char ch = expr[i];

        if (isspace(ch)) continue;  // Ignore spaces

        if (isalnum(ch)) {  // Operand (variable or number)
            string operand(1, ch);
            operandStack.push_back(dag.createNode(operand));
        } else if (operators.count(ch)) {  // Operator
            while (!operatorStack.empty() && operators.count(operatorStack.back())) {
                char op = operatorStack.back();
                operatorStack.pop_back();

                Node* right = operandStack.back();
                operandStack.pop_back();
                Node* left = operandStack.back();
                operandStack.pop_back();

                operandStack.push_back(dag.createNode(string(1, op), left, right));
            }
            operatorStack.push_back(ch);
        }
    }

    while (!operatorStack.empty()) {
        char op = operatorStack.back();
        operatorStack.pop_back();

        Node* right = operandStack.back();
        operandStack.pop_back();
        Node* left = operandStack.back();
        operandStack.pop_back();

        operandStack.push_back(dag.createNode(string(1, op), left, right));
    }

    return operandStack.empty() ? nullptr : operandStack.back();
}

// **Main Function**
int main() {
    string expression;
    cout << "Enter an arithmetic expression: ";
    cin >> expression;

    DAG dag;
    Node* root = constructDAG(expression, dag);

    cout << "\nDAG representation (inorder traversal): ";
    dag.inorder(root);
    cout << endl;

    return 0;
}
