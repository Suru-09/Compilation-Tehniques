#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include<memory>

#include "Token.hpp"
#include "Logger.hpp"


class Node {
public:
    Token token;
    std::shared_ptr<Node> next;
    Node(Token& token);

    Node& operator=(const Node& node );

    Node() = default;
    ~Node() = default;

private:
    std::string class_name;
    Logger logger;    
};

#endif