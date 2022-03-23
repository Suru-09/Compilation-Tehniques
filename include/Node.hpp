#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include "Token.hpp"
#include<memory>

class Node {
public:
    Token token;
    std::shared_ptr<Node> next;
    Node(Token& token);

    Node& operator=(const Node& node );

    Node() = default;
    ~Node() = default;
    
};

#endif