#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <unordered_set>
#include <queue>
#include <string>
using namespace std;

// Structure for a DAG node
struct Node {
    string value;
    Node *left, *right;
    int id;
    Node(string val, int nodeId) : value(val), left(nullptr), right(nullptr), id(nodeId) {}
};

// Class to construct and manage the DAG
class DAG {
    map<string, Node*> nodeMap;
    int nextId = 0;
    unordered_set<Node*> visited;

public:
    Node* createNode(string value, Node* left = nullptr, Node* right = nullptr) {
        string key = value;
        if (left) key += to_string(left->id);
        if (right) key += to_string(right->id);

        if (nodeMap.count(key)) return nodeMap[key];
        
        Node* newNode = new Node(value, nextId++);
        newNode->left = left;
        newNode->right = right;
        nodeMap[key] = newNode;
        return newNode;
    }

    // Print inorder traversal
    void inorder(Node* root) {
        if (!root) return;
        if (root->left) cout << "(";
        inorder(root->left);
        cout << root->value;
        inorder(root->right);
        if (root->right) cout << ")";
    }

    // Show DAG structure
    void showDAG(Node* root) {
        if (!root) return;
        
        visited.clear();
        cout << "\nNodes:\n------------------\n";
        printNodes(root);
        
        visited.clear();
        cout << "\nConnections:\n------------------\n";
        printConnections(root);
    }

private:
    void printNodes(Node* node) {
        if (!node || visited.count(node)) return;
        visited.insert(node);
        cout << "Node " << node->id << ": " << node->value << endl;
        if (node->left) printNodes(node->left);
        if (node->right) printNodes(node->right);
    }
    
    void printConnections(Node* node) {
        if (!node || visited.count(node)) return;
        visited.insert(node);
        
        if (node->left) {
            cout << node->id << " (" << node->value << ") → " 
                 << node->left->id << " (" << node->left->value << ")" << endl;
            printConnections(node->left);
        }
        
        if (node->right) {
            cout << node->id << " (" << node->value << ") → " 
                 << node->right->id << " (" << node->right->value << ")" << endl;
            printConnections(node->right);
        }
    }
};

// Parse expression and build DAG
Node* buildDAG(string expr, DAG &dag) {
    vector<Node*> operands;
    vector<char> operators;
    set<char> ops = {'+', '-', '*', '/'};

    for (char ch : expr) {
        if (isspace(ch)) continue;
        
        if (isalnum(ch)) {
            operands.push_back(dag.createNode(string(1, ch)));
        } 
        else if (ops.count(ch)) {
            while (!operators.empty() && ops.count(operators.back())) {
                char op = operators.back();
                operators.pop_back();

                Node* right = operands.back();
                operands.pop_back();
                Node* left = operands.back();
                operands.pop_back();

                operands.push_back(dag.createNode(string(1, op), left, right));
            }
            operators.push_back(ch);
        }
    }

    while (!operators.empty()) {
        char op = operators.back();
        operators.pop_back();

        Node* right = operands.back();
        operands.pop_back();
        Node* left = operands.back();
        operands.pop_back();

        operands.push_back(dag.createNode(string(1, op), left, right));
    }

    return operands.empty() ? nullptr : operands.back();
}

int main() {
    string expression;
    cout << "Enter an arithmetic expression: ";
    cin >> expression;

    DAG dag;
    Node* root = buildDAG(expression, dag);

    cout << "\nInorder traversal: ";
    dag.inorder(root);
    cout << endl;
    
    dag.showDAG(root);
    
    return 0;
}