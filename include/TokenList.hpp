#ifndef TOKENLIST_HPP
#define TOKENLIST_HPP

#include<iostream>
#include "Node.hpp"

class TokenList {
public:
    ~TokenList() = default;
    TokenList() : head(nullptr) {}
    
    void push(Node& node);

private:
    std::shared_ptr<Node> head;

};

#endif