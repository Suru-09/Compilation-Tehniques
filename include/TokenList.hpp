#ifndef TOKENLIST_HPP
#define TOKENLIST_HPP

#include<iostream>
#include "Node.hpp"

class TokenList {
public:  
    TokenList() : head{nullptr} {}
    void push(Node& node);

private:
    std::unique_ptr<Node> head;

};

#endif