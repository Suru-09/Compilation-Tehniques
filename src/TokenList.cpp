#include "TokenList.hpp"
#include "LexicalAnalyzer.hpp"

void TokenList::push(Node& node) {

    auto temp{std::make_shared<Node>(std::move(node))};

    // std::cout << "\nSPL: " << temp->token.code << "\n";

    if(last) {
        last->next = temp;
        //last = std::move(temp);  
    }
    else if(!last) {
        head = temp;
        //last = head;
    }
    last = std::move(temp);
    // std::cout << "HEAD: " << head->token.code << "\n";
    // std::cout << "LAST: " << last->token.code << "\n";
}

void TokenList::print_list() {
    LexicalAnalyzer lexic;

    auto copy = head;
    while(copy) {
        std::cout << "Token with code: " << lexic.print_pretty(copy->token.code) << "\nLine: " << copy->token.line << "\n";
        try {
            std::string text = std::get<std::string> (copy->token.text);
            std::cout << "Text: " << text << "\n\n";
        }
        catch(...) {
            try {
                long int integer = std::get<long int> (copy->token.text);
                std::cout << "Long int: " << integer << "\n\n";
            }
            catch(...) {
                try {
                    double dbl = std::get<double> (copy->token.text);
                    std::cout << "Double : " << dbl << "\n\n";
                }
                catch(...) {
                    std::cout << "Error while printing the list of tokens!\n";
                }
            }
        }
        
        copy = copy->next;
    }
}

TokenList::TokenList() 
: head(nullptr),
last(nullptr)
{}