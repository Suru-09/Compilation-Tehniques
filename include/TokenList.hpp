#ifndef TOKENLIST_HPP
#define TOKENLIST_HPP

#include<iostream>

#include "Token.hpp"
#include "Node.hpp"

class TokenList {
public:
    ~TokenList() = default;
    TokenList();
    
    void push(Node& node);
    std::shared_ptr<Node> get_head();
    void print_list();

private:
    std::shared_ptr<Node> head, last;
    std::string class_name{""};
    Logger logger;
};

#endif