#include "Node.hpp"

Node::Node(Token& token) 
: token{std::move(token)},
next{nullptr},
class_name("Node")
{}

Node& Node::operator=(const Node& node ) {
        token = node.token;
        return *this;
}