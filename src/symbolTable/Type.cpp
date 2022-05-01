#include "Type.hpp"

Type::Type(const int& type_base, const std::string& symbol_name, const int& elements) 
: type_base(type_base),
symbol_name(symbol_name),
elements(elements)
{}

Type& Type::operator=(const Type& type) {
    if( this != &type ) {
        type_base = type.type_base;
        symbol_name = type.symbol_name;
        elements = type.elements;
    }
    return *this;
}