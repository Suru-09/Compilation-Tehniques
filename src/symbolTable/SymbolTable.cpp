#include "SymbolTable.hpp"
#include "VariadicTable.hpp"
#include "Utils.hpp"


SymbolTable::SymbolTable() 
: class_name("SymbolTable")
{
    logger = Logger{class_name};
}

void SymbolTable::add_symbol(Symbol& symbol) {
    symbol_table.insert(std::make_pair(symbol.get_name(), symbol));
    std::cout << logger << "[ADDING] key with value: " << symbol.get_name()
        << " from depth: " << symbol.depth << "\n";
    print_symbol_table();
}

Symbol SymbolTable::find_symbol(const std::string& key) {
    // std::cout << logger << "Am intrat in find_symbol: " << key << "\n";
    auto it = symbol_table.find(key);
    Symbol temp;

    if(it != symbol_table.end()) {
        if(symbol_table.count(key) == 1) {
            return (*it).second;
        }
        temp = (*it).second;
    } 
    else {
        return temp;
    }

    while (it != symbol_table.end()) {
        if(temp.depth <= (*it).second.depth) {
            temp = (*it).second;
        }
        it++;
    }

    std::cout << logger << "[FIND] temp.name = " << temp.get_name() << "\n";
    return temp;
}

void SymbolTable::update_symbol(Symbol symbol) {
    auto it = symbol_table.find(symbol.name);

    if(it != symbol_table.end()) {
        (*it).second = symbol;
        std::cout << logger << " [UPDATED] Symbol with name: " << symbol.name << " at depth: "
            << symbol.depth << "\n";
    }
    else {
        std::cout << logger << " [UPDATE FAILED] Symbol with name: " << symbol.name << " at depth: "
            << symbol.depth << "\n";
    }
}

void SymbolTable::delete_symbol(const std::string& key) {
    auto it = symbol_table.find(key);

    if(it != symbol_table.end()) {
        if(symbol_table.count(key) == 1) {
            symbol_table.erase(it);
        }
    }

    auto copy = it;
    while (it != symbol_table.end()) {
        if( (*copy).second.depth <= (*it).second.depth) {
            copy = it;
        }
        ++it;
    }

    std::cout << logger << "[DELETE] key with value: " << (*copy).second.get_name()
        << " from depth: " << (*copy).second.depth << "\n";
    symbol_table.erase(copy);
}

void SymbolTable::delete_symbols_from_given_level(const int& level) {

   auto x = symbol_table.begin();
   for( ; x != symbol_table.end(); ) {
       if ( (*x).second.depth == level) {
            std::cout << logger << "[DELETE] key with value: " << (*x).second.get_name()
            << " from depth: " << (*x).second.depth << "\n";
           x = symbol_table.erase(x);
       }
       else  {
           x++;
       }
   }

   std::cout << logger << "Deletion on level: " << level << " was succesfull!\n";
}

bool SymbolTable::symbol_exists(const std::string& key) {
    if ( find_symbol(key).name != "" ) {
        return true;
    }
    return false;
}

void SymbolTable::print_symbol_table() {;
    std::vector<std::string> headers{"Count", "Name", "Class", "TB Type",
        "IS_ARRAY", "Depth", "Members"};
    VariadicTable<int, std::string, std::string, std::string, std::string, int, std::string> vt(headers);
    vt.setColumnFormat({VariadicTableColumnFormat::AUTO,
                        VariadicTableColumnFormat::AUTO,
                        VariadicTableColumnFormat::AUTO,
                        VariadicTableColumnFormat::AUTO,
                        VariadicTableColumnFormat::AUTO,
                        VariadicTableColumnFormat::AUTO,
                        VariadicTableColumnFormat::AUTO});

    std::cout << logger << " Printing the symbol table: \n";
    int counter = 0;
    for(auto x: symbol_table) {
        counter++;
        std::string name = x.first;
        std::string class_ = utils::class_to_string(x.second.class_);
        std::string type = utils::type_to_string(x.second.type.type_base);
        int depth = x.second.depth;
        bool is_array = false;;
        if(x.second.type.elements >= 0) {
            is_array = true;
        }
        std::string is_arr;
        if(is_array)
            is_arr = "TRUE";
        else 
            is_arr = "FALSE";
        std::string member = "";
        for(auto y: x.second.members) {
            member += y.second.name + ", ";
            // std::cout << logger << "HEHEHE: " << y.second.name << "\n";
        }
        vt.addRow(counter, name, class_, type, is_arr, depth, member);
    }
    vt.print(std::cout);
}