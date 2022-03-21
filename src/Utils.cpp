#include<iostream>
#include<fstream>
#include<string>
#include <vector>
#include<stdlib.h>

#include "Token.hpp"
#include "TokenList.hpp"
#include "Node.hpp"

namespace utils {

    std::vector<std::string> read_file(const std::string& file_name) {
        std::vector<std::string> arr;
        std::string temp;

        std::ifstream in(file_name);
        if(!in) {
            std::cout << "There was an error while opening the file with name: " <<
                file_name;
        }

        while(1) {
            in >> temp;
            arr.push_back(temp);
            if(in.eof())
                break;
        }
        
        return arr;
    }

    void test_token_list() {
        
    {
    TokenList list;
    
    Token token1(2, 3L, 4);
    Token token2(2, 3L, 5);
    Token token3(2, 3L, 6);
    Token token4(2, 3L, 7);
    Token token5(2, 3L, 8);
    Token token6(2, 3L, 9);
    Token token7(2, 3L, 10);
    Token token8(2, 3L, 11);

    Node node1(token1);
    Node node2(token1);
    Node node3(token1);
    Node node4(token1);
    Node node5(token1);
    Node node6(token1);
    Node node7(token1);
    Node node8(token1);

    list.push(node1);
    list.push(node2);
    list.push(node3);
    list.push(node4);
    list.push(node5);
    list.push(node6);
    list.push(node7);
    list.push(node8);
    }
    
    std::cout << "I am out of scope!\n";
    }
}