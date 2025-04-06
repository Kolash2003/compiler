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
    string value;  // Operator or operand
    Node *left, *right;  // Children nodes
    int id;  // Unique identifier for the node

    Node(string val, int nodeId) : value(val), left(nullptr), right(nullptr), id(nodeId) {}
};

// Class to construct and manage the DAG
class DAG {
    map<string, Node*> nodeMap;  // Store already created nodes (for common subexpression elimination)
    int nextId;  // For assigning unique IDs to nodes
    
    // Set to track visited nodes during traversal (to handle shared nodes)
    unordered_set<Node*> visited;

public:
    DAG() : nextId(0) {}

    Node* createNode(string value, Node* left = nullptr, Node* right = nullptr) {
        string key = value;
        if (left) key += to_string(left->id);
        if (right) key += to_string(right->id);

        // If node already exists (common subexpression elimination)
        if (nodeMap.count(key)) return nodeMap[key];

        // Create new node
        Node* newNode = new Node(value, nextId++);
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

    // Visualize the DAG structure with node IDs and links
    void visualizeDAG(Node* root) {
        if (!root) return;
        
        // Clear visited set for a new traversal
        visited.clear();
        
        // 1. First print all nodes with their IDs
        cout << "Nodes:\n";
        cout << "----------------\n";
        cout << "ID | Value\n";
        cout << "----------------\n";
        printAllNodes(root);
        
        // 2. Then print all edges
        cout << "\nLinks:\n";
        cout << "----------------\n";
        cout << "From -> To\n";
        cout << "----------------\n";
        
        // Clear visited set for a new traversal
        visited.clear();
        printAllLinks(root);
    }

private:
    // Print all nodes in the DAG
    void printAllNodes(Node* node) {
        if (!node || visited.count(node)) return;
        
        // Mark as visited
        visited.insert(node);
        
        // Print node info
        cout << node->id << " | " << node->value << endl;
        
        // Recursively print children
        if (node->left) printAllNodes(node->left);
        if (node->right) printAllNodes(node->right);
    }
    
    // Print all links in the DAG
    void printAllLinks(Node* node) {
        if (!node || visited.count(node)) return;
        
        // Mark as visited
        visited.insert(node);
        
        // Print links to children
        if (node->left) {
            cout << node->id << " (" << node->value << ") -> " 
                 << node->left->id << " (" << node->left->value << ")" << endl;
            printAllLinks(node->left);
        }
        
        if (node->right) {
            cout << node->id << " (" << node->value << ") -> " 
                 << node->right->id << " (" << node->right->value << ")" << endl;
            printAllLinks(node->right);
        }
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

// Print ASCII representation of DAG
void printDAGStructure(Node* root) {
    // Use a set to track visited nodes
    unordered_set<Node*> visited;
    queue<Node*> queue;
    
    if (root) queue.push(root);
    
    cout << "\nDAG Structure (ASCII Representation):\n";
    cout << "===================================\n";
    
    // Keep track of nodes at each level for better visualization
    while (!queue.empty()) {
        int levelSize = queue.size();
        
        for (int i = 0; i < levelSize; i++) {
            Node* node = queue.front();
            queue.pop();
            
            if (visited.count(node)) {
                // If already visited, just mark as reference
                cout << "[REF to Node " << node->id << "] ";
                continue;
            }
            visited.insert(node);
            
            // Print node with connections
            cout << "Node " << node->id << " (" << node->value << ")";
            
            // Show connections
            if (node->left || node->right) {
                cout << " → [";
                if (node->left) {
                    cout << "L: " << node->left->id << " (" << node->left->value << ")";
                    queue.push(node->left);
                }
                
                if (node->left && node->right) cout << ", ";
                
                if (node->right) {
                    cout << "R: " << node->right->id << " (" << node->right->value << ")";
                    queue.push(node->right);
                }
                cout << "]";
            }
            
            cout << "   ";
        }
        cout << "\n";
    }
    cout << "===================================\n";
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
    
    cout << "\nDetailed DAG Structure:\n";
    dag.visualizeDAG(root);
    
    // Print ASCII representation of the DAG
    printDAGStructure(root);

    return 0;
}