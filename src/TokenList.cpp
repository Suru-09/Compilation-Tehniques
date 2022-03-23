#include "TokenList.hpp"

void TokenList::push(Node& node) {

    auto temp{std::make_shared<Node>(std::move(node))};
    if(head) {
        temp->next = std::move(head);
        head = std::move(temp);
    }
    else {
        head = std::move(temp);    
    }
}

void TokenList::print_list() {
    auto copy = head;
    while(copy) {
        std::cout << "Token with code: " << copy->token.code << "\nLine: " << copy->token.line << "\n";
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
: head(nullptr) 
{}