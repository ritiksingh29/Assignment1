#include "crow_all.h" // Crow HTTP framework
#include <iostream>
#include <string>
#include <memory>
#include <map>
#include <stack>
#include <sstream>
#include <vector>
using namespace std;

// Define the structure of an AST node
struct Node
{
    string type;            // "operator" or "operand"
    string value;           // Value for operand nodes
    shared_ptr<Node> left;  // Left child for the AST
    shared_ptr<Node> right; // Right child for the AST

    Node(const string &type, const string &value = "")
        : type(type), value(value), left(nullptr), right(nullptr) {}
};

// Create a new node
shared_ptr<Node> create_node(const string &type, const string &value = "")
{
    return make_shared<Node>(type, value);
}

// Helper function to build the AST from a rule string
shared_ptr<Node> create_rule(const string &rule_string)
{
    stack<shared_ptr<Node>> operands;
    stack<shared_ptr<Node>> operators;
    istringstream iss(rule_string);
    string token;

    while (iss >> token)
    {
        if (token == "AND" || token == "OR")
        {
            operators.push(create_node("operator", token));
        }
        else if (token == "(" || token == ")")
        {
            // Parentheses are handled implicitly by using stacks, but not implemented for nesting
        }
        else
        {
            operands.push(create_node("operand", token));
        }

        if (operands.size() >= 2 && !operators.empty())
        {
            auto right = operands.top();
            operands.pop();
            auto left = operands.top();
            operands.pop();
            auto op = operators.top();
            operators.pop();

            op->left = left;
            op->right = right;
            operands.push(op);
        }
    }

    return operands.top(); // Return the root of the AST
}

// Combine multiple rules into a single AST
shared_ptr<Node> combine_rules(const vector<string> &rule_strings)
{
    shared_ptr<Node> combined_ast = nullptr;

    for (const auto &rule : rule_strings)
    {
        auto new_ast = create_rule(rule);
        if (combined_ast == nullptr)
        {
            combined_ast = new_ast;
        }
        else
        {
            // Combine rules using "OR"
            auto or_node = create_node("operator", "OR");
            or_node->left = combined_ast;
            or_node->right = new_ast;
            combined_ast = or_node;
        }
    }

    return combined_ast;
}

// Evaluate the AST against user data
bool evaluate_rule(const shared_ptr<Node> &ast, const map<string, string> &data)
{
    if (ast->type == "operand")
    {
        string attribute = ast->value.substr(0, ast->value.find(' '));
        string comparison = ast->value.substr(ast->value.find(' ') + 1);

        if (attribute == "age" && comparison == "> 30")
        {
            return stoi(data.at("age")) > 30;
        }
        else if (attribute == "salary" && comparison == "> 50000")
        {
            return stoi(data.at("salary")) > 50000;
        }
        else if (attribute == "department" && comparison == "= 'Sales'")
        {
            return data.at("department") == "Sales";
        }
        // Add more condition parsing as needed
    }
    else if (ast->type == "operator")
    {
        if (ast->value == "AND")
        {
            return evaluate_rule(ast->left, data) && evaluate_rule(ast->right, data);
        }
        else if (ast->value == "OR")
        {
            return evaluate_rule(ast->left, data) || evaluate_rule(ast->right, data);
        }
    }
    return false;
}

// In-memory rule storage (can be extended to store multiple rules)
shared_ptr<Node> current_rule = nullptr;

int main()
{
    crow::SimpleApp app;

    // Route to create a new rule
    CROW_ROUTE(app, "/api/create_rule").methods("POST"_method)([](const crow::request &req)
                                                               {
        auto body = crow::json::load(req.body);
        if (!body)
            return crow::response(400, "Invalid request body");

        string rule_string = body["rule"].s();
        current_rule = create_rule(rule_string);

        return crow::response(200, crow::json::wvalue{{"message", "Rule created successfully"}}); });

    // Route to evaluate the rule against user data
    CROW_ROUTE(app, "/api/evaluate_rule").methods("POST"_method)([](const crow::request &req)
                                                                 {
        auto body = crow::json::load(req.body);
        if (!body)
            return crow::response(400, "Invalid request body");

        map<string, string> user_data;
        for (auto &item : body["user_data"].items())
        {
            user_data[item.key()] = item.second.s();
        }

        bool result = evaluate_rule(current_rule, user_data);
        return crow::response(200, crow::json::wvalue{{"result", result}}); });

    app.port(8080).multithreaded().run();
}
