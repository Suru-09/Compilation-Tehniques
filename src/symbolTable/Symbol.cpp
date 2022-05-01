#include "Symbol.hpp"
#include<iostream>

std::string Symbol::get_name() {
    return name;
}

Symbol::Symbol(const std::string& name, const int& class_, const Type& type) 
: class_name("Symbol"),
name(name),
class_(class_),
type(type),
depth(0),
memory_zone(0)
{
    logger = Logger{class_name};
}

Symbol::Symbol()
: class_name("Symbol"),
name(""),
class_(0),
depth(0),
memory_zone(0)
{
    logger = Logger{class_name};
}

// void Symbol::set_members(std::vector<std::string> members_) {
//     if(class_ == CLS_FUNC) {
//         std::make_pair<int, std::vector<std::string>>(CLS_FUNC, std::move(members_));
//     }
//     else if(class_ == CLS_STRUCT) {
//         std::make_pair<int, std::vector<std::string>>(CLS_STRUCT, std::move(members_));
//     }
//     else {
//         std::cout << logger << " Wrong type given in set_members() function!\n";
//         exit(-1);
//     }
// }

bool Symbol::add_member(Symbol s) {
    if(class_ == CLS_FUNC) {
        auto pair = std::make_pair<int, Symbol>(CLS_FUNC, std::move(s));
        members.push_back(pair);
    }
    else if(class_ == CLS_STRUCT) {
        auto pair = std::make_pair<int, Symbol>(CLS_STRUCT, std::move(s));
        members.push_back(pair);
    }
    else {
        std::cout << logger << " Wrong type given in add_member() function!\n";
        exit(-1);
    }
}

Symbol& Symbol::operator=(const Symbol& symbol) {
    if( this != &symbol ) {
        name = symbol.name;
        class_ = symbol.class_;
        memory_zone = symbol.memory_zone;
        depth = symbol.depth;
        type = symbol.type;

        members.clear();
        for(auto x: symbol.members) {
            members.push_back(x);
        }
    }
    return *this;
}