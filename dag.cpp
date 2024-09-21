#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <queue>
#include <functional>

using namespace std;

class Node {
    private:
        string operation;
        double value; // Used if the node is a contant
        vector<Node*> inputs; // pointers to operand nodes
    public:
        // Constructor for contants
        Node(double val) {
            this->value = val;
            operation = "";
        }

        // Constructor for operators
        Node(string op) {
            this->operation = op;
            value = 0;
        }

        // Check if the node is an operator
        bool is_operator() const {
            return !operation.empty();
        }

        ~Node() {}

        // Getter for the operation
        string get_operation() const {
            return operation;
        }

        // Getter for the value (used for constants)
        double get_value() const {
            return value;
        }

        // Getter for inputs
        vector<Node*>& get_inputs() {
            return inputs;
        }

        // Set inputs
        void set_inputs(const vector<Node*>& input_nodes) {
            inputs = input_nodes;
        }

};

// Topological sort helper function
vector<Node*> topological_sort(Node* root) {
    unordered_map<Node*, int> in_degree;
    queue<Node*> zero_in_degree;
    vector<Node*> sorted_nodes;
    unordered_map<Node*, vector<Node*>> adj_list;

    // Build the graph and calculate in-degree of each node
    function<void(Node*)> dfs = [&](Node* node) {
        if(in_degree.count(node)) {
            return;
        }
        in_degree[node] = 0;
        for(auto input : node->get_inputs()) {
            if(adj_list[input].empty()) {
                adj_list[input].push_back(node);
                in_degree[node]++;
                dfs(input);
            }
        }
    };

    dfs(root);

    // Push nodes with zero in-degree into the queue
    for(auto& [node,degree] : in_degree) {
        if(degree==0) {
            zero_in_degree.push(node);
        }
    }

    // Process graph in topological order
    while(!zero_in_degree.empty()) {
        Node* node = zero_in_degree.front();
        zero_in_degree.pop();
        sorted_nodes.push_back(node);

        for(Node* dependent : adj_list[node]) {
            in_degree[dependent]--;
            if(in_degree[dependent] == 0) {
                zero_in_degree.push(dependent);
            }
        }
    }

    return sorted_nodes;
}

// Evaluate DAG by processing each node in topological order
double evaluate(Node* root) {
    auto sorted_nodes = topological_sort(root);

    // Store the evaluated value for each node
    unordered_map<Node*, double> evaluated_values;

    for(auto &node : sorted_nodes) {
        if(!node->is_operator()) {
            evaluated_values[node] = node->get_value();
        } else {
            // if it's an operator, evaluate its input first
            double left_value = evaluated_values[node->get_inputs()[0]];
            double right_value = evaluated_values[node->get_inputs()[1]];

            if(node->get_operation() == "+") {
                evaluated_values[node] = left_value + right_value;
            } else if(node->get_operation() == "-") {
                evaluated_values[node] = left_value - right_value;
            } else if(node->get_operation() == "*") {
                evaluated_values[node] = left_value * right_value;
            } else if(node->get_operation() == "/") {
                evaluated_values[node] = left_value / right_value;
            }
        }
    }

    // The result of the computation is the value of the root node
    return evaluated_values[root];
}

int main() {

    // Create nodes for the DAG
    Node* n1 = new Node(3);
    Node* n2 = new Node(4);
    Node* n3 = new Node(5);

    Node* plus_node = new Node("+");
    Node* mult_node = new Node("*");

    // Connect nodes to for a DAG
    plus_node->set_inputs({n1, n2});
    mult_node->set_inputs({plus_node, n3});

    double result = evaluate(mult_node);
    cout<< "The result of the expression is: " <<
    result<<
    endl;

    // Clean up dynamic memory
    delete n1;
    delete n2;
    delete n3;
    delete plus_node;
    delete mult_node;

    return 0;
}