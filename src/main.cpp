#include<iostream>
#include "TokenList.cpp"
#include "Token.hpp"
#include "Node.cpp"
#include "ReadFile.cpp"


int main() {

    std::cout << "Print something so that I know my script is working!\n";
    auto arr = utils::read_file("../testing_files/file1.txt");

    for(auto const &x: arr)
        std::cout << x << " ";
    

    return 0;
}