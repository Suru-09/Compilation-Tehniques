#include "SymbolTable.hpp"
#include "VariadicTable.hpp"


SymbolTable::SymbolTable() 
: class_name("SymbolTable")
{
    logger = Logger{class_name};
}

void SymbolTable::add_symbol(Symbol& symbol) {
    symbol_table.insert(std::make_pair(symbol.get_name(), symbol));
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
   for( ; x != symbol_table.end(); ++x ) {
       if ( (*x).second.depth == level) {
            std::cout << logger << "[DELETE] key with value: " << (*x).second.get_name()
            << " from depth: " << (*x).second.depth << "\n";
           x = symbol_table.erase(x);
           x--;
       }
   }

   std::cout << logger << "Deletion on level: " << level << " was succesfull!\n";
}

void SymbolTable::print_symbol_table() {;
    std::vector<std::string> headers{"Count", "Name", "Class", "TB Type", "ARRAY (bool)", "Depth"};
    VariadicTable<int, std::string, int, int, bool, int> vt(headers);
    vt.setColumnFormat({VariadicTableColumnFormat::AUTO,
                        VariadicTableColumnFormat::SCIENTIFIC,
                        VariadicTableColumnFormat::FIXED,
                        VariadicTableColumnFormat::PERCENT,
                        VariadicTableColumnFormat::PERCENT,
                        VariadicTableColumnFormat::PERCENT});

    std::cout << logger << " Printing the symbol table: \n";
    int counter = 0;
    for(auto x: symbol_table) {
        counter++;
        std::string name = x.first;
        int class_ = x.second.class_;
        int type = x.second.type.type_base;
        int depth = x.second.depth;
        bool is_array = false;;
        if(x.second.type.elements >= 0) {
            is_array = true;
        }
        vt.addRow(counter, name, class_, type, is_array, depth);
    }
    vt.print(std::cout);
}