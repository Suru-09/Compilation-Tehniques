#include "TokenList.hpp"
#include "LexicalAnalyzer.hpp"
#include "VariadicTable.hpp"

#include<string>
#include<vector>
#include<iostream>

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

    std::vector<std::string> headers{"Count", "Code", "Text", "Line"};
    VariadicTable<int, std::string, std::string, int> vt(headers);
    int counter = 0;

    auto copy = head;
    while(copy) {
        counter++;
        std::string code = lexic.print_pretty(copy->token.code);
        int line = copy->token.line;
        //std::cout << "Token with code: " << lexic.print_pretty(copy->token.code) << "\nLine: " << copy->token.line << "\n";
        try {
            std::string text = std::get<std::string> (copy->token.text);
            if(text == "")
                text = "---";
            //std::cout << "Text: " << text << "\n\n";
            vt.addRow(counter, code, text , line);
        }
        catch(...) {
            try {
                long int integer = std::get<long int> (copy->token.text);
                //std::cout << "Long int: " << integer << "\n\n";
                vt.addRow(counter, code, std::to_string(integer) , line);
            }
            catch(...) {
                try {
                    double dbl = std::get<double> (copy->token.text);
                    // std::cout << "Double : " << dbl << "\n\n";
                    vt.addRow(counter, code, std::to_string(dbl) , line);
                }
                catch(...) {
                    std::cout << "Error while printing the list of tokens!\n";
                }
            }
        }
        
        copy = copy->next;
    }

    vt.print(std::cout);
}

TokenList::TokenList() 
: head(nullptr),
last(nullptr)
{}