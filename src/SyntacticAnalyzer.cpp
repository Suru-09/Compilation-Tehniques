#include "SyntacticAnalyzer.hpp"
#include "Symbol.hpp"

SyntacticAnalyzer::SyntacticAnalyzer(const LexicalAnalyzer& lex_analiz) 
: lex(lex_analiz),
class_name("SyntacticAnalyzer"),
current_depth(0),
current_struct(""),
current_func(""),
is_struct(false)
{   
    current_token = lex.token_list.get_head();
    logger = Logger{class_name};
}

SyntacticAnalyzer::SyntacticAnalyzer() 
: class_name("SyntacticAnalyzer"),
current_depth(0),
current_struct(""),
current_func(""),
is_struct(false)
{   
    logger = Logger{class_name};
}

int SyntacticAnalyzer::expr() {
    consumed_token = std::make_shared<Node> (*current_token);
    if(expr_assign()) {
        return 1;
    }
    else {
        current_token = consumed_token;
        return 0;
    }
}

int SyntacticAnalyzer::match(const int& code) {
    if(current_token && current_token->token.code == code) {
        try {
            long a = 0;
            double b = 0;
            std::string s = "";
            switch(code) {
                case 34:    // CT_INT
                    a = std::get<long>(current_token->token.text);
                    ret_val.set_constant_value(a);
                    break;
                case 44:    // CT_REAL
                    b = std::get<double>(current_token->token.text);
                    ret_val.set_constant_value(b);
                    break;
                case 51:    // CT_CHAR
                    a = std::get<long>(current_token->token.text);
                    ret_val.set_constant_value(a);
                    break;
                case 55:    // CT_STRING
                    s = std::get<std::string> (current_token->token.text);
                    ret_val.set_constant_value(s);
                    break;
                default:
                    break;
            }
        }
        catch(int sth) {

        }

        current_token = current_token->next;
        return 1;
    }
    return 0;
}

int SyntacticAnalyzer::r_while() {
    if(!match(lex.WHILE)) {
        return 0;
    }

    if(!match(lex.LPAR)) {
        std::cout << logger << utils::log_error(current_token->token.line, "Missing ( ");
        exit(lex.LPAR);
    }

    if(!expr()) {
        std::cout << logger << utils::log_error(current_token->token.line, "Invalid expression after (");
        exit(-1);
    }

    if(!match(lex.RPAR)) {
        std::cout << logger << utils::log_error(current_token->token.line, "Missing ) ");
        exit(lex.RPAR);
    }

    if(!stm()) {
        std::cout << logger << utils::log_error(current_token->token.line, "Missing statement ");
        exit(-1);
    }
    return 1;
}

int SyntacticAnalyzer::r_return() {
    if(!match(lex.RETURN)) {
        return 0;
    }

    expr();

    if(!match(lex.SEMICOLON)) {
        std::cout << logger << utils::log_error(current_token->token.line, "Missing ; ");
        exit(lex.SEMICOLON);
    }

    return 1;
}


int SyntacticAnalyzer::stm() {

    consumed_token = std::make_shared<Node>(*current_token);

    if(r_optional_expr() == 1) {
        return 1;        
    }
    else if(stm_block() == 1) {
        return 1;
    }
    else if(r_while() == 1) {
        std::cout << logger << "Am gasit while!\n";
        return 1;
    }
    else if(r_return() == 1) {
        std::cout << logger << "Found RETURN statement!\n";
        return 1;
    }
    else if(r_for() == 1) {
        std::cout << logger << "Found FOR statement!\n";
        return 1;
    }
    else if(r_if() == 1) {
        std::cout << logger << "Found IF statement!\n";
        return 1;
    }
    else if(r_break() == 1) {
        std::cout << logger << "Found BREAK statement!\n";
        return 1;
    }
    current_token = consumed_token;
    return 0;
}

int SyntacticAnalyzer::stm_block() {

    if( match(lex.SEMICOLON) ) {
        return 1;
    }

    if(!match(lex.LACC)) {
        return 0;
    }

    current_depth++;

   while( stm() || decl_var() ) {}

    if(!match(lex.RACC)) {
        std::cout << logger << utils::log_error(current_token->token.line, "Missing } ");
        exit(lex.RACC);
    }

    symbol_table.delete_symbols_from_given_level(current_depth--);

    return 1;
}

void SyntacticAnalyzer::unit() {
    while(1) {
        if(current_token->token.code == lex.END) {
            symbol_table.print_symbol_table();
            break;
        }

        if(decl_func()) {}
        else if(decl_struct() ) {}
        else if( decl_var() ) {}
    }
}

void SyntacticAnalyzer::check_if_struct_exists() {
    if(current_token->token.code == lex.ID) {
        std::string possible_key = std::get<std::string> (current_token->token.text);
        tmp = symbol_table.find_symbol(possible_key);

        if( tmp.name == "" ) {
            if(current_token->next && current_token->next->token.code != lex.LACC ) {
                std::cout << logger << utils::log_error(current_token->token.line, "This struct hasn't been defined yet!");
                exit(lex.ID);
            }
        }
        
        // if( tmp.class_ != tmp.CLS_STRUCT ) {
        //     std::string s = tmp.name + " is not a CT_STRUCT!";
        //     std::cout << logger << utils::log_error(current_token->token.line, s);
        //     exit(lex.ID);
        // }
    }
}


int SyntacticAnalyzer::type_base() {
    consumed_token = std::make_shared<Node>(*current_token);

    if(match(lex.INT)) {
        return lex.INT;
    }
    else if(match(lex.DOUBLE)) {
        return lex.DOUBLE;
    }
    else if(match(lex.CHAR)) {
        return lex.CHAR;
    }
    else if(match(lex.STRUCT)) {
        check_if_struct_exists();
        if( current_token->token.code == lex.ID && is_struct ) {
            std::cout << logger << "Eu sunt s: " << std::get<std::string> (current_token->token.text)<< "\n";
            current_struct = std::get<std::string> (current_token->token.text);
        }

        if(match(lex.ID)) {
            return lex.STRUCT;
        }
        else {
            current_token = consumed_token;
            return 0;
        }
    }
    return 0;
}

int SyntacticAnalyzer::array_decl() {

    if(!match(lex.LBRACKET)) {
        if(tmp.name != "" && symbol_table.find_symbol(tmp.name).name == "") {
            std::cout << logger << "[ARRAY DECLARAY] eu sunt: " << current_struct << "\n";
            if(current_struct != "") {
                auto symb = symbol_table.find_symbol(current_struct);
                std::cout << logger << "[ARRAY DECL CONT] eu sunt: " << symb.name << "\n";
                if(symb.name != "") {
                    for(auto x: symb.members) {
                        if(x.second.name == tmp.name) {
                            std::cout << logger << utils::log_error(current_token->token.line, " Symbol redefinition in [STRUCT] ");
                            exit(lex.ID);
                        }
                    }
    
                    symb.add_member(tmp);
                }
                else {
                    std::cout << logger << utils::log_error(current_token->token.line, "An ERROR occurred because current_struct isn't SET RIGHT!");
                    exit(lex.ID);
                }
                symbol_table.update_symbol(symb);
            }
            else {
                add_var(tmp.type, tmp.name);
            }
        }
        return 0;
    }

    expr();
    if(tmp.name != "" && symbol_table.find_symbol(tmp.name).name == "") {
        std::cout << logger << "[ARRAY DECLARAY] eu sunt: " << current_struct << "\n";
        if(current_struct != "") {
            auto symb = symbol_table.find_symbol(current_struct);
            std::cout << logger << "[ARRAY DECL CONT] eu sunt: " << symb.name << "\n";
            if(symb.name != "") {
                for(auto x: symb.members) {
                    if(x.second.name == tmp.name) {
                        std::cout << logger << utils::log_error(current_token->token.line, " Symbol redefinition in [STRUCT] ");
                        exit(lex.ID);
                    }
                }
                symb.add_member(tmp);
            }
            else {
                std::cout << logger << utils::log_error(current_token->token.line, "An ERROR occurred because current_struct isn't SET RIGHT!");
                exit(lex.ID);
            }
            symbol_table.update_symbol(symb);
        }
        else {
            // TO DO: TYPE CHECKING ADD REAL VALUE FOR SIZE
            tmp.type.elements = 1;
            add_var(tmp.type, tmp.name);
        }
    }

    if(!match(lex.RBRACKET)) {
        std::cout << logger << utils::log_error(current_token->token.line, "Missing ]");
        exit(lex.RBRACKET);
    }

    std::cout << logger << "Found an ARRAY ! \n";
    return 1;
}

int SyntacticAnalyzer::type_name() {
    if(!type_base()) {
        return 0;
    }

    array_decl();

    return 1;
}

Symbol SyntacticAnalyzer::add_decl_var_to_symbol(Type& type) {
    Symbol sym;
    if( current_token && current_token->token.code == lex.ID ) {
        std::string possible_key = std::get<std::string> (current_token->token.text);
        sym = symbol_table.find_symbol(possible_key);
        
        auto val = current_token->next;
        if( sym.name != "" && val->token.code != lex.LPAR) {
            std::cout << logger << utils::log_error(current_token->token.line, "Symbol redefiniton ");
            exit(lex.ID);
        }
        sym.name = possible_key;
        sym.class_ = sym.CLS_STRUCT;
        sym.depth = current_depth;
        sym.type = type;
    }
    return sym;
}

Type SyntacticAnalyzer::type_base_condition() {
    Type type;
    if( current_token ) {
        int v = current_token->token.code;
            // TO DO: possible nullptr exception be careful!!!!!! ( current_token->next )
        if( v == lex.INT ) {
            type.type_base = type.TB_INT;
            type.elements = -1;
        }
        else if( v == lex.DOUBLE ) {
            type.type_base = type.TB_DOUBLE;
            type.elements = -1;
        }
        else if( v == lex.CHAR ) {
            type.type_base = type.TB_CHAR;
            type.elements = -1;
        }
        else if( v == lex.STRUCT && current_token->next->token.code == lex.ID ) {
            type.type_base = type.TB_STRUCT;
            type.elements = -1;
            type.symbol_name = std::get<std::string> (current_token->next->token.text);
        }
        else if( v == lex.VOID ) {
            type.type_base = type.TB_VOID;
            type.elements = -1;
        }
    }
    return type;
}

int SyntacticAnalyzer::decl_var() {
    consumed_token = std::make_shared<Node>(*current_token);
    auto type = type_base_condition();

    if(!type_base() ) {
        return 0;
    }

    if(match(lex.MUL)) {
        current_token = consumed_token;
        return 0;
    }

    // std::cout << logger << "ID-ul pe care il verific inainte sa ies: " << std::get<std::string> (current_token->token.text) << "\n";
    tmp = add_decl_var_to_symbol(type);

    if(!match(lex.ID)) {
        std::cout << logger << utils::log_error(current_token->token.line, "Missing ID ");
        exit(lex.ID);
    }

    array_decl();

    while(1) {
        if(!match(lex.COMMA)) {
            break;
        }

        tmp = add_decl_var_to_symbol(type);

        if(!match(lex.ID)) {
            std::cout << logger << utils::log_error(current_token->token.line, "Missing ID ");
            exit(lex.ID);
        }

        array_decl();
    }

    if(match(lex.LPAR)) {
        current_token = consumed_token;
        return 0;
    }

    if(!match(lex.SEMICOLON)) {
        std::cout << logger << utils::log_error(current_token->token.line, "Missing SEMICOLON ");
        exit(lex.SEMICOLON);
    }

    std::cout << logger << "Found a VARIABLE!\n";
    return 1;
}

void SyntacticAnalyzer::check_struct_helper() {
    auto val = current_token->next->next;
    if( val && val->token.code == lex.LACC) {
        if(current_token && current_token->token.code == lex.STRUCT) {
            if( current_token->next && current_token->next->token.code == lex.ID ) {
                Type type = type_base_condition();
                std::string possible_key = std::get<std::string> (current_token->next->token.text);
                Symbol sym;
                sym = symbol_table.find_symbol(possible_key);
                if( sym.name != "") {
                    std::cout << logger << utils::log_error(current_token->token.line, "1Symbol redefinition ");
                    exit(lex.ID);
                }
                sym.name = possible_key;
                sym.type = type;
                sym.class_ = sym.CLS_STRUCT;
                sym.depth = current_depth;
                symbol_table.add_symbol(sym);
            }
        }
    }
}

int SyntacticAnalyzer::decl_struct() {
    consumed_token = std::make_shared<Node>(*current_token);

    check_struct_helper();
    is_struct = true;

   if(!type_base()) {
        current_token = consumed_token;
        is_struct = false;
        current_struct = "";
        return 0;
   }
   
   if(match(lex.ID)) {
        current_token = consumed_token;
        is_struct = false;
        current_struct = "";
        return 0;
   }

    if(!match(lex.LACC)) {
        current_token = consumed_token;
        if(!decl_var()) {
            std::cout << logger << utils::log_error(current_token->token.line, "Missing { ");
            exit(lex.LACC);
        }
        is_struct = false;
        current_struct = "";
        return 0;
    }

    current_depth++;
    std::cout << "Eu sunt current_struct: " << current_struct << "\n";
    while( decl_var() ) {}
    current_depth--;

    if(!match(lex.RACC)) {
        std::cout << logger << utils::log_error(current_token->token.line, "Missing } ");
        exit(lex.RACC);
    }

    if(!match(lex.SEMICOLON)) {
        std::cout << logger << utils::log_error(current_token->token.line, "Missing SEMICOLON ");
        exit(lex.SEMICOLON);
    }

    current_struct = "";
    is_struct = false;

    std::cout << logger << "Found a STRUCT!\n";
    return 1;
}

bool SyntacticAnalyzer::add_args_symbol_table(Type& type) {
    if (current_func != "") {
        Symbol sym = symbol_table.find_symbol(current_func);
        if(sym.name != "") {
            if(current_token && current_token->token.code == lex.ID) {
                std::string name = std::get<std::string> (current_token->token.text);
                tmp.name = name;
                tmp.depth = current_depth;
                tmp.type = type;
                tmp.memory_zone = sym.MEM_ARG;

                sym.add_member(tmp);
                symbol_table.update_symbol(sym);
                return true;
            }
        }
    }   
    return false;
}
int SyntacticAnalyzer::func_arg() {

    auto type = type_base_condition();
    if(!type_base()) {
        return 0;
    }

    add_args_symbol_table(type);

    if(!match(lex.ID)) {
        std::cout << logger << utils::log_error(current_token->token.line, "Missing ID ");
        exit(lex.ID);
    }

    array_decl();

    return 1;
}

Symbol SyntacticAnalyzer::check_decl_func_helper(Type& type) {
    if(current_token && current_token->token.code == lex.ID) {
        std::string possible_key = std::get<std::string> (current_token->token.text);
        tmp = symbol_table.find_symbol(possible_key);

        if( tmp.name != "") {
            std::cout << logger << utils::log_error(current_token->token.line, "Symbol redefinition in func decl");
            exit(lex.ID);
        }

        tmp.type = type;
        tmp.name = possible_key;
        tmp.class_ = tmp.CLS_FUNC;
        tmp.depth = current_depth;
    }

    return tmp;
}

int SyntacticAnalyzer::decl_func() {
    consumed_token = std::make_shared<Node>(*current_token);
    auto type = type_base_condition();

    if( type_base()) {
        match(lex.MUL);
    }
    else if(!match(lex.VOID)) {
        return 0;
    }

    if (match(lex.LACC)) {
        current_token = consumed_token;
        return 0;
    }

    auto sym = check_decl_func_helper(type);

    if(!match(lex.ID)) {
        std::cout << logger << utils::log_error(current_token->token.line, "Missing ID ");
        exit(lex.ID);
    }

    if (match(lex.SEMICOLON) || match(lex.LBRACKET)) {
        current_token = consumed_token;
        return 0;
    }

    symbol_table.add_symbol(sym);
    current_func = sym.name;
    // !!! BE CAREFUL with this
    current_depth++;

    if(!match(lex.LPAR)) {
        std::cout << logger << utils::log_error(current_token->token.line, "Missing ( ");
        exit(lex.LPAR);
    }

    while(1) {
        std::cout << logger << "[APELEZ] FUNC_ARG!\n";
        if(func_arg() == 1) {
            if(!match(lex.COMMA) && !func_arg()) 
                break;

            if(match(lex.COMMA) && !func_arg()) {
                std::cout << logger << utils::log_error(current_token->token.line, "Missing arguments ");
                exit(lex.COMMA);
            }
        }
        else {
            break;
        }
    }

    if(!match(lex.RPAR)) {
        std::cout << logger << utils::log_error(current_token->token.line, "Missing ) ");
        exit(lex.RPAR);
    }

    // !!! BE CAREFUL with this
    current_func = "";

    if(!stm_block()) {
        std::cout << logger << utils::log_error(current_token->token.line, "Creating a function without a body ");
        exit(-1);
    }

    symbol_table.delete_symbols_from_given_level(current_depth);
    current_depth--;
    std::cout << logger << "Found a FUNCTION!\n";
    return 1;
}

int SyntacticAnalyzer::r_for() {

    if(!match(lex.FOR)) {
        return 0;
    }

    if(!match(lex.LPAR)) {
        std::cout << logger << utils::log_error(current_token->token.line, "Missing ( ");
        exit(lex.LPAR);
    }

    int cnt = 3;
    while(cnt--) {
        //std::cout << logger << "  " << cnt << '\n';
        expr();

        if(cnt > 0) {
            if(!match(lex.SEMICOLON)) {
                std::cout << logger << utils::log_error(current_token->token.line, "Missing ; in for ");
                exit(lex.SEMICOLON);
            }
        }
    }

    if(!match(lex.RPAR)) {
        std::cout << logger << utils::log_error(current_token->token.line, "Missing ) ");
        exit(lex.RPAR);
    }

    if(!stm()) {
        std::cout << logger << utils::log_error(current_token->token.line, "Missing statement ");
        exit(-1);
    }

    return 1;
}

int SyntacticAnalyzer::r_if() {

    if(!match(lex.IF)) {
        return 0;
    }

    if(!match(lex.LPAR)) {
        std::cout << logger << utils::log_error(current_token->token.line, "Missing ( ");
        exit(lex.LPAR);
    }

    if(!expr()) {
        std::cout << logger << utils::log_error(current_token->token.line, "Missing expression ");
        exit(-1);       
    }

    if(!match(lex.RPAR)) {
        std::cout << logger << utils::log_error(current_token->token.line, "Missing ) ");
        exit(lex.RPAR);
    }

    if(!stm()) {
        std::cout << logger << utils::log_error(current_token->token.line, "Missing statement ");
        exit(-1);
    }

    if(match(lex.ELSE)) {
        if(!stm()) {
            std::cout << logger << utils::log_error(current_token->token.line, "Else with empty body ");
            exit(lex.ELSE);
        }
    }

    return 1;
}

int SyntacticAnalyzer::r_break() {

    if(!match(lex.BREAK)) {
        return 0;
    }

    if(!match(lex.SEMICOLON)) {
        std::cout << logger << utils::log_error(current_token->token.line, "Missing ; in for ");
        exit(lex.SEMICOLON);
    }

    return 1;

}

int SyntacticAnalyzer::r_optional_expr() {
    consumed_token = std::make_shared<Node> (*current_token);

    expr();

    if(!match(lex.SEMICOLON)) {
        current_token = consumed_token;
        return 0;   
    }

    return 1;
}

Symbol SyntacticAnalyzer::tc_search_function() {
    if(current_token && current_token->token.code == lex.ID && current_token->next &&
        current_token->next->token.code == lex.LPAR ) {
        std::string possible_key = std::get<std::string>(current_token->token.text);
        auto sym = symbol_table.find_symbol(possible_key);
        if (sym.name == "") {
            std::cout << logger << utils::log_error(current_token->token.line, "Function called not found in ST!");
            exit(lex.ID);
        }

        if (sym.class_ != sym.CLS_FUNC && sym.class_ != sym.CLS_EXTFUNC) {
            std::cout << logger << utils::log_error(current_token->token.line, "Call to non function!");
            exit(lex.ID);
        }

        ret_val.type = sym.type;
        ret_val.is_constant_value = false;
        ret_val.is_left_value = true;
        return sym;
    }
    return Symbol{};
}

void SyntacticAnalyzer::tc_check_function(Symbol& symbol) {

}

int SyntacticAnalyzer::expr_primary() {
    consumed_token = std::make_shared<Node> (*current_token);

    if(match(lex.CT_INT)) {
        ret_val.type = create_type(ret_val.type.TB_INT, -1);
        ret_val.is_left_value = false;
        ret_val.is_constant_value = true;
        std::cout << logger << "Found a PRIMARY (CT_INT) !\n";
        return 1;
    }
    else if(match(lex.CT_REAL)){
        ret_val.type = create_type(ret_val.type.TB_DOUBLE, -1);
        ret_val.is_left_value = false;
        ret_val.is_constant_value = true;
        std::cout << logger << "Found a PRIMARY (CT_REAL) \n";
        return 1;
    }
    else if(match(lex.CT_CHAR)) {
        ret_val.type = create_type(ret_val.type.TB_CHAR, -1);
        ret_val.is_left_value = false;
        ret_val.is_constant_value = true;
        std::cout << logger << "Found a PRIMARY (CT_CHAR) !\n";
        return 1;
    }
    else if(match(lex.CT_STRING)) {
        ret_val.type = create_type(ret_val.type.TB_STRING, -1);
        ret_val.is_left_value = false;
        ret_val.is_constant_value = true;
        std::cout << logger << "Found a PRIMARY (CT_STRING)!\n";
        return 1;
    }
    else if(match(lex.LPAR)){
        if(!type_name()) {
            if(expr()) {
                if(match(lex.RPAR)) {
                    std::cout << logger << "Found a PRIMARY ( LPAR EXPR RPAR)!\n";
                    return 1;
                }
            }
        }
        else if( match(lex.RPAR) ) {

            while(expr_cast()) {}
            std::cout << logger << "Found a CAST EXPRESSION! \n";
            return 1;
        }
    }

    current_token = consumed_token;
    auto symb = tc_search_function();

    auto next = current_token->next;
    if ( current_token && current_token->token.code == lex.ID  &&
        next && next->token.code != lex.LPAR ) {
        std::string possible_key = std::get<std::string> (current_token->token.text);
        auto s = symbol_table.find_symbol(possible_key);
        if( s.name == "") {
            std::cout << logger << utils::log_error(current_token->token.line, "[ID] hasn't been defined!");
            exit(lex.ID);
        }
        ret_val.type = s.type;
        ret_val.is_left_value = true;
        ret_val.is_constant_value = false;
    }

    if(!match(lex.ID)) {
        return 0;
    }

    auto vec = symb.members;
    auto it = 0;
    auto size = vec.size();

    if(match(lex.LPAR) ) {
        int val = expr();

        if(size) {
            if( it >= size ) {
                std::cout << logger << utils::log_error(current_token->token.line, "Too many arguments in function1!");
                exit(lex.LPAR);
            }
            cast_type(vec[it].second.type, ret_val.type);
            ++it;
        }

        while(match(lex.COMMA) && expr()) {
            if(size) {
                if( it >= size ) {
                    std::cout << logger << utils::log_error(current_token->token.line, "Too many arguments in function2!");
                    exit(lex.LPAR);
                }
                cast_type(vec[it].second.type, ret_val.type);
                ++it;
            }
        }
    
        if(!match(lex.COMMA) && !expr()) {
        }
        else {
            if(match(lex.COMMA) && !expr()) {
                std::cout << logger << utils::log_error(current_token->token.line, "Missing COMMA ");
                exit(lex.COMMA);
            }

            if(!match(lex.COMMA) && expr()) {
                std::cout << logger << utils::log_error(current_token->token.line, "Missing expression ");
                exit(-1);
            }
        }

        if(!match(lex.RPAR) ) {
            std::cout << logger << utils::log_error(current_token->token.line, "Missing ) ");
            exit(lex.RPAR);
        }

        if( it < size && size) {
            std::cout << logger << utils::log_error(current_token->token.line, "Too little arguments in function!");
            exit(lex.RPAR);
        }
    
        ret_val.type = symb.type;
        ret_val.is_constant_value = false;
        ret_val.is_left_value = false;                 
    }
    else {
        consumed_token = std::make_shared<Node>(*current_token);
        if(match(lex.LBRACKET) == 1) {
            current_token = consumed_token;
            if(expr_postfix_bracket() == 1) {
                return 1;
            }
        }
        else if(match(lex.DOT) == 1) {
            current_token = consumed_token;
            if(expr_postfix_bracket() == 1) {
                return 1;
            }
        }
    }

    std::cout << logger << "Found a PRIMARY EXPRESSION!\n";
    return 1;
}

bool SyntacticAnalyzer::find_val_in_members(const Symbol& symb, const std::string& symb_name) {
    std::cout << logger << "[FIND_VAL_IN_MEMBERS]: STRUCT/FUNCTION: " << symb.name 
        << " searched SYMBOL: " << symb_name << "\n";
    auto vec = symb.members;
    for(const auto& x: vec) {
        if (x.second.name == symb_name) {
            return true;
        } 
    }
    return false;
}

void SyntacticAnalyzer::check_postfix(const ReturnValue& rv) {
    if (current_token && current_token->token.code == lex.ID) {
        auto symbol = symbol_table.find_symbol(rv.type.symbol_name); // Search for struct variable
        if ( symbol.name == "" ) {
            std::string s = "Couldn't find any STRUCT VAR declared with the name: [" 
                + rv.type.symbol_name + "]";
            std::cout << logger << utils::log_error(current_token->token.line, s);
        }
        auto struct_ = symbol_table.find_symbol(symbol.type.symbol_name);   // Search after actual struct saved
        
        std::string name = std::get<std::string> (current_token->token.text);
        bool ok = find_val_in_members(struct_, name);
        if ( !ok ) {
            std::string s = "This STRUCT [" + struct_.name + "] " + "doesn't have " + name + " as parameter ";
            std::cout << logger << utils::log_error(current_token->token.line, s);
            exit(lex.ID);
        }
        ret_val.type = symbol.type;
        ret_val.is_left_value = true;
        ret_val.is_constant_value = false;
    }
}

int SyntacticAnalyzer::expr_postfix_bracket() {

    consumed_token = std::make_shared<Node> (*current_token);

    if(expr_primary()) {
        return 1;
    }

    if(!match(lex.LBRACKET)) {
        ReturnValue ret = ret_val;

        if(!match(lex.DOT)) {
            current_token = consumed_token;
            return 0;
        }
    
        check_postfix(ret);

        if(!match(lex.ID)) {
            current_token = consumed_token;
            return 0;
        }
        expr_postfix_bracket();
    }
    else {
        if(ret_val.type.elements < 0) {
            std::cout << logger << utils::log_error(current_token->token.line, "[NON-ARRAY] can't be index ");
            exit(lex.LBRACKET);
        }

        ReturnValue val;
        val = ret_val;

        int cnt = 0 ;
        while(expr()) {
            ++cnt;
        }

        Type t = create_type(t.TB_INT, -1);
        std::cout << logger << "RET_VAL: " << ret_val.type.type_base << "   T: " << t.type_base << "\n";
        std::cout << logger << "RET_VAL: " << ret_val.type.elements << "   T: " << t.elements << "\n";
        std::cout << logger << "RET_VAL: " << ret_val.type.symbol_name << "   T: " << t.symbol_name << "\n";

        cast_type(ret_val.type, t);
        ret_val.type = ret_val.type;
        ret_val.type.elements = -1;
        ret_val.is_left_value = true;
        ret_val.is_constant_value = false;

        if(!cnt) {
            std::cout << logger << utils::log_error(current_token->token.line, "Missing EXPR in [ ]");
            exit(lex.RBRACKET);
        }
        
        if(!match(lex.RBRACKET)) {
            current_token = consumed_token;
            return 0;
        }
        expr_postfix_bracket();
    }
    return 1;
}

int SyntacticAnalyzer::expr_postfix() {
   consumed_token = std::make_shared<Node> (*current_token);

   if(!expr_postfix_bracket()){
       current_token = consumed_token;
       return 0;
   }
    
    // std::cout << logger << "Found a POSTFIX!\n";
    return 1;
}

int SyntacticAnalyzer::expr_unary() {
    consumed_token = std::make_shared<Node> (*current_token);

     if( expr_postfix() ) {
        return 1;
    }

    if( match(lex.SUB) ) {
       if( !expr_unary()) {
           current_token = consumed_token;
           return 0;
       }
       return 1;
    }
     if( match(lex.NOT) ) {
        if( !expr_unary()) {
           current_token = consumed_token;
           return 0;
       }
       return 1;
    }
    
    current_token = consumed_token;
    return 0;
}

int SyntacticAnalyzer::expr_cast() {
    consumed_token = std::make_shared<Node> (*current_token);

    if(expr_unary()) {
        return 1;
    }

    if(!match(lex.LPAR)) {
        return 0;
    }

    if(!type_name()) {
        std::cout << logger << utils::log_error(current_token->token.line, "Missing type  ");
        exit(-1);       
    }

    if(!match(lex.RPAR)) {
        std::cout << logger << utils::log_error(current_token->token.line, "Missing ) ");
        exit(lex.RPAR);
    }

    if(expr_cast()) {
        return 1;
    }

    return 0;
}

int SyntacticAnalyzer::expr_mul_helper() {
    consumed_token = std::make_shared<Node> (*current_token);
    
    if(match(lex.MUL) || match(lex.DIV) ) {
        if(!expr_cast()) {
            current_token = consumed_token;
            return 0;
        }
    }
    else {
        current_token = consumed_token;
        return 0;
    }
   
    while(expr_mul_helper() ) {}
    return 1;
}


int SyntacticAnalyzer::expr_mul() {
    consumed_token = std::make_shared<Node> (*current_token);
    int val = expr_cast();
    
    if(!expr_mul_helper()) {
        if(val == 1) {
            return 1;
        }
        else {
            current_token = consumed_token;
            return 0;
        }
    }
    else {
        std::cout << logger << "Found a MUL expression!\n";
        return 1;
    }
    
    return 0;
}


int SyntacticAnalyzer::expr_add_helper() {
    consumed_token = std::make_shared<Node> (*current_token);
    
    if(match(lex.ADD) || match(lex.SUB) ) {
        if(!expr_mul()) {
            current_token = consumed_token;
            return 0;
        }
    }
    else {
        current_token = consumed_token;
        return 0;
    }
   
    while(expr_add_helper() ) {}
    return 1;
}


int SyntacticAnalyzer::expr_add() {
    consumed_token = std::make_shared<Node> (*current_token);
    int val = expr_mul();
    
    if(!expr_add_helper()) {
        if(val == 1) {
            return 1;
        }
        else {
            current_token = consumed_token;
            return 0;
        }
    }
    else {
        std::cout << logger << "Found a ADD expression!\n";
        return 1;
    }
    
    return 0;
}

int SyntacticAnalyzer::expr_rel_helper() {
    consumed_token = std::make_shared<Node> (*current_token);
    
    if(match(lex.LESS) || match(lex.LESSEQ) || match(lex.GREATER) || match(lex.GREATEREQ) ) {
        if(!expr_add()) {
            current_token = consumed_token;
            return 0;
        }
    }
    else {
        current_token = consumed_token;
        return 0;
    }
   
    while(expr_rel_helper() ) {}
    return 1;
}


int SyntacticAnalyzer::expr_rel() {
    consumed_token = std::make_shared<Node> (*current_token);
    int val = expr_add();
    
    if(!expr_rel_helper()) {
        if(val == 1) {
            return 1;
        }
        else {
            current_token = consumed_token;
            return 0;
        }
    }
    else {
        std::cout << logger << "Found a REL expression!\n";
        return 1;
    }
    
    return 0;
}

int SyntacticAnalyzer::expr_eq_helper() {
    consumed_token = std::make_shared<Node> (*current_token);
    
    if(match(lex.EQUAL) || match(lex.NOTEQ)  ) {
        if(!expr_rel()) {
            current_token = consumed_token;
            return 0;
        }
    }
    else {
        current_token = consumed_token;
        return 0;
    }
   
    while(expr_eq_helper() ) {}

    return 1;
}


int SyntacticAnalyzer::expr_eq() {
    consumed_token = std::make_shared<Node> (*current_token);
    int val = expr_rel();
    
    if(!expr_eq_helper()) {
        if(val == 1) {
            return 1;
        }
        else {
            current_token = consumed_token;
            return 0;
        }
    }
    else {
        std::cout << logger << "Found an EQ expression!\n";
        return 1;
    }
    
    return 0;
}

int SyntacticAnalyzer::expr_and_helper() {
    consumed_token = std::make_shared<Node> (*current_token);
    
    if(match(lex.AND)) {
        if(!expr_eq()) {
            current_token = consumed_token;
            return 0;
        }
    }
    else {
        current_token = consumed_token;
        return 0;
    }
   
    while(expr_and_helper() ) {}

    return 1;
}


int SyntacticAnalyzer::expr_and() {
    consumed_token = std::make_shared<Node> (*current_token);
    int val = expr_eq();
    
    if(!expr_and_helper()) {
        if(val == 1) {
            return 1;
        }
        else {
            current_token = consumed_token;
            return 0;
        }
    }
    else {
        std::cout << logger << "Found an AND expression!\n";
        return 1;
    }
    
    return 0;
}

int SyntacticAnalyzer::expr_or_helper() {
    consumed_token = std::make_shared<Node> (*current_token);
    
    if(match(lex.OR) ) {
        if(!expr_and()) {
            current_token = consumed_token;
            return 0;
        }
    }
    else {
        current_token = consumed_token;
        return 0;
    }
   
    while(expr_or_helper() ) {}

    return 1;
}


int SyntacticAnalyzer::expr_or() {
    consumed_token = std::make_shared<Node> (*current_token);
    int val = expr_and();
    
    if(!expr_or_helper()) {
        if(val == 1) {
            return 1;
        }
        else {
            current_token = consumed_token;
            return 0;
        }
    }
    else {
        std::cout << logger << "Found an OR expression!\n";
        return 1;
    }
    
    return 0;
}

int SyntacticAnalyzer::expr_assign_helper() {
    consumed_token = std::make_shared<Node> (*current_token);
    
    if( match(lex.ASSIGN) ) {
        if( !expr_assign()) {
            current_token = consumed_token;
            return 0;
        }
    }
    else {
        current_token = consumed_token;
        return 0;
    }

    while(expr_eq_helper() ) {}
    return 1;
}

int SyntacticAnalyzer::expr_assign() {
    consumed_token = std::make_shared<Node> (*current_token);
    int val = expr_or();
    
    if(!expr_assign_helper()) {
        if(val == 1) {
            return 1;
        }
        else {
            current_token = consumed_token;
            return 0;
        }
    }
    else {
        std::cout << logger << "Found an ASSIGN expression!\n";
        return 1;
    }
    return 0;
}

bool SyntacticAnalyzer::add_var(Type type, std::string name) {
    Symbol sym;
    if( current_struct != "" ) {
        sym = symbol_table.find_symbol(name);
        if( sym.name == "") {
            sym.name = name;
            sym.class_  = sym.CLS_VAR;
            sym.type = type;
            sym.depth = current_depth;
            symbol_table.add_symbol(sym);
            return true;
        }
        else {
            std::cout << logger << utils::log_error(current_token->token.line, "2Symbol redefinition while adding ");
            exit(lex.ID);
        }
    }
    else if( current_func != "" ) {
        sym = symbol_table.find_symbol(name);
        if( sym.name != "" && sym.depth == current_depth ) {
            std::cout << logger << utils::log_error(current_token->token.line, "3Symbol redefinition while adding ");
            exit(lex.ID);
        }
        sym.name = name;
        sym.class_  = sym.CLS_VAR;
        sym.memory_zone = sym.MEM_LOCAL;
        sym.depth = current_depth;
        sym.type = type;
        symbol_table.add_symbol(sym);
    }
    else {
        sym = symbol_table.find_symbol(name);
        if( sym.name != "") {
            std::cout << logger << utils::log_error(current_token->token.line, "4Symbol redefinition while adding ");
            exit(lex.ID);
        }
        sym.name = name;
        sym.class_ = sym.CLS_VAR;
        sym.memory_zone = sym.MEM_GLOBAL;
        sym.depth = current_depth;
        sym.type = type;
        symbol_table.add_symbol(sym);
    }

    return true;
}

void SyntacticAnalyzer::cast_type(const Type& src, const Type& dest) {
    std::cout << logger << "[CAST] intre [" << src.type_base << "] si [" << dest.type_base << "]\n";
    if(src.elements >= 0) {
        if(dest.elements >= 0) {
            if(src.type_base != dest.type_base) {
                std::cout << logger << utils::log_error(current_token->token.line, "An array cannot be converted to an array of another type!");
                exit(lex.ID);
            }
        }
        else {
            std::cout << logger << utils::log_error(current_token->token.line, "An array cannot be converted to non-array!");
            exit(lex.ID);                
        }
    }
    else if(dest.elements >= 0) {
            std::cout << logger << utils::log_error(current_token->token.line, "A non-array cannot be converted to an array!");
            exit(lex.ID);          
    }

    switch(src.type_base) {
        case 0: // TB_INT
        case 1: //  TB_DOUBLE
        case 2: //  TB_CHAR
        switch(dest.type_base) {
            case 0: // TB_INT
            case 1: //  TB_DOUBLE
            case 2: //  TB_CHAR
                return;
        }
        case 3: // TB_STRUCT
            if(dest.type_base == dest.TB_STRUCT) {
                // TO DO: Modify this to correspond in future
                if(src.symbol_name != dest.symbol_name) {
                    std::cout << logger << utils::log_error(current_token->token.line, "A struct cannot be converted to another struct!\n");
                    exit(lex.ID);         
                }
                return;
            }
    }
    std::cout << logger << utils::log_error(current_token->token.line, "Incompatible types!\n");
    exit(lex.ID);    
}

Type SyntacticAnalyzer::get_arithmetic_type(const Type& type1, const Type& type2) {
    if (type1.type_base == type1.TB_INT) {
        if ( type2.type_base == type1.TB_CHAR ) {
            return type1;
        }
        return type2;
    }
    else if(type1.type_base == type1.TB_CHAR) {
        if ( type2.type_base == type1.TB_CHAR ) {
            return type1;
        }
        return type2;
    }
    else if(type1.type_base == type1.TB_DOUBLE) {
        return type1;
    }
    else if(type1.type_base == type1.TB_STRUCT) {
        return type1;
    }

    std::cout << logger << utils::log_error(current_token->token.line, "Couldn't get Arithmetic Type!\n");
    exit(lex.ID);
}

Type SyntacticAnalyzer::create_type(const int& type_base, const int& elements) {
    Type t;
    t.type_base = type_base;
    t.elements = elements;
    return t;
}
