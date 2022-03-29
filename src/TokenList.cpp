#include "TokenList.hpp"
#include "LexicalAnalyzer.hpp"
#include "VariadicTable.hpp"

#include<string>
#include<vector>
#include<iostream>
#include<iomanip>
#include <sstream>

void TokenList::push(Node& node) {

    auto temp{std::make_shared<Node>(std::move(node))};
    if(last) {
        last->next = temp;
    }
    else if(!last) {
        head = temp;
    }
    last = std::move(temp);
}

void TokenList::print_list() {
    LexicalAnalyzer lexic;

    std::vector<std::string> headers{"Count", "Code", "Text", "Line"};
    VariadicTable<int, std::string, std::string, int> vt(headers);
    vt.setColumnFormat({VariadicTableColumnFormat::AUTO,
                        VariadicTableColumnFormat::SCIENTIFIC,
                        VariadicTableColumnFormat::FIXED,
                        VariadicTableColumnFormat::PERCENT});

    int counter = 0;

    auto copy = head;
    while(copy) {
        counter++;
        std::string code = lexic.print_pretty(copy->token.code);
        int line = copy->token.line;
        // std::cout << logger << "Token with code: " << lexic.print_pretty(copy->token.code) << "\nLine: " << copy->token.line << "\n";
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
                // std::cout << logger << "Long int: " << integer << "\n\n";
                vt.addRow(counter, code, std::to_string(integer) , line);
            }
            catch(...) {
                try {
                    double dbl = std::get<double> (copy->token.text);
                    // std::cout << logger << "Double : " << dbl << "\n\n";
                    std::ostringstream strs;
                    strs  << std::setprecision(15) << dbl;
                    std::string str = strs.str();
                    vt.addRow(counter, code, str , line);
                }
                catch(...) {
                    // std::cout << logger << "Error while printing the list of tokens!\n";
                }
            }
        }
        copy = copy->next;
    }
    vt.print(std::cout);
}

TokenList::TokenList() 
: head(nullptr),
last(nullptr),
class_name("TokenList")
{
    logger = Logger{class_name};
}