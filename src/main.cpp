#include<iostream>
#include<map>


#include "TokenList.hpp"
#include "Token.hpp"
#include "Node.hpp"
#include "Utils.hpp"
#include "LexicalAnalyzer.hpp"


int main() {

    std::cout << "Print something so that I know my script is working!\n";
    auto arr = utils::read_file("../testing_files/ID.txt");

    int i = 0;
    for(auto const &x: arr) {
        std::cout << i << " " << x << "    ";
        ++i;
    }
    std::cout << "\n\n";
        

    LexicalAnalyzer lexical_analyzer{arr};
    // std::cout << lexical_analyzer.get_next_token();
    // std::cout << lexical_analyzer.get_next_token();
    lexical_analyzer.get_all_tokens();    


    return 0;
}