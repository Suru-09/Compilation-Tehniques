#ifndef TOKENLIST_HPP
#define TOKENLIST_HPP

#include<iostream>
#include "Node.hpp"

class TokenList {
public:
    ~TokenList() = default;
    TokenList();
    
    void push(Node& node);
    void print_list();

private:
    std::shared_ptr<Node> head;
};

#endif