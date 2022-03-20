#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include "Token.hpp"
#include<memory>

class Node {

private:
    Token token;
public:
    std::shared_ptr<Node> next;
    Node(Token token) : token{std::move(token)}, next{nullptr} {}

    explicit Node();
    ~Node() = default;
    
};

#endif