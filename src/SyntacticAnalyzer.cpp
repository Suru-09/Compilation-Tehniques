#include "SyntacticAnalyzer.hpp"
#include "Symbol.hpp"

SyntacticAnalyzer::SyntacticAnalyzer(const LexicalAnalyzer& lex_analiz) 
: lex(lex_analiz),
class_name("SyntacticAnalyzer"),
current_depth(0),
current_struct(""),
current_func(""),
is_struct(false),
ret_val(ReturnValue{})
{   
    current_token = lex.token_list.get_head();
    logger = Logger{class_name};
    vm = VirtualMachine{};
    add_predefined_functions();
}

SyntacticAnalyzer::SyntacticAnalyzer() 
: class_name("SyntacticAnalyzer"),
current_depth(0),
current_struct(""),
current_func(""),
is_struct(false),
ret_val(ReturnValue{})
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
                case 36:    // CT_INT
                    try {
                        s = std::get<std::string>(current_token->token.text);
                        a = stoi(s);
                        ret_val.set_constant_value(a);
                    }
                    catch(const std::exception& e) {
                        try {
                            a = std::get<long> (current_token->token.text);
                            ret_val.set_constant_value(a);
                        }
                        catch(const std::exception& e2) {}
                    }
                    break;
                case 44:    // CT_REAL
                    try {
                        s = std::get<std::string>(current_token->token.text);
                        b = stod(s);
                        ret_val.set_constant_value(b);
                    }
                    catch(const std::exception& e) {
                        try {
                            b = std::get<double> (current_token->token.text);
                            ret_val.set_constant_value(b);
                        }
                        catch(const std::exception& e2) {}
                    }
                    break;
                case 51:    // CT_CHAR
                    s = std::get<std::string>(current_token->token.text);
                    ret_val.set_constant_value(s);
                    break;
                case 55:    // CT_STRING
                    s = std::get<std::string> (current_token->token.text);
                    ret_val.set_constant_value(s);
                    break;
                default:
                    break;
            }
        }
        catch(int sth) {}

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

    check_r_if();

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

void SyntacticAnalyzer::check_r_return() {
    if (current_func != "") {
        auto sym = symbol_table.find_symbol(current_func);
        if (sym.name == "") {
            std::cout << logger << utils::log_error(current_token->token.line, "[CURRENT_FUNCTION] != EMPTY but lacks from ST!");
            exit(lex.ID);
        }
        if (sym.type.type_base == sym.type.TB_VOID) {
            std::cout << logger << utils::log_error(current_token->token.line, "[VOID FUNCTION] cannot return anything!");
            exit(lex.ID);
        }
        cast_type(ret_val.type, sym.type);
    }
    else {
        std::cout << logger << "[CURRENT_FUNCT] == EMPTY \n";
    }
}

int SyntacticAnalyzer::r_return() {
    if(!match(lex.RETURN)) {
        return 0;
    }

    if ( expr() ) {
        check_r_return();
    }
    
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
        // else {
        //     std::cout << logger << utils::log_error(current_token->token.line, "Program has been exited due to an unknown symbol!");
        //     break;
        // }

        // std::cout << logger << "[CURRENT_TOKEN]: " << lex.print_pretty(current_token->token.code) << "\n";
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

void SyntacticAnalyzer::check_array_decl() {
    // if ( !ret_val.is_constant_value ) {
    //     std::string s = "The [ARRAY SIZE] is not a CONSTANT!";
    //     std::cout << logger << utils::log_error(current_token->token.line, s);
    //     exit(lex.ID);
    // }

    if ( ret_val.type.type_base != ret_val.type.TB_INT ) {
        std::string s = "The [ARRAY SIZE] is not an INTEGER!";
        std::cout << logger << utils::log_error(current_token->token.line, s);
        exit(lex.ID);
    }
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
        else {
            if( tmp.name != "" && symbol_table.find_symbol(tmp.name).name != "" && tmp.type.symbol_name == "FUNC_ARG") {
                std::string s = "Symbol [" + tmp.name + "] with type: [" 
                    + utils::type_to_string(tmp.type.type_base) + "] was already defined in the function arguments!";
                std::cout << logger << utils::log_error(current_token->token.line, s);
                exit(lex.ID);
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
                std::cout << logger << "[RET_VAL:] " << std::get<long> (ret_val.constant_value) << "\n";
                check_array_decl();
                try {
                    tmp.type.elements = std::get<long> (ret_val.constant_value);
                }
                catch(const std::exception& e) {}
                symb.add_member(tmp);
            }
            else {
                std::cout << logger << utils::log_error(current_token->token.line, "An ERROR occurred because current_struct isn't SET RIGHT!");
                exit(lex.ID);
            }
            symbol_table.update_symbol(symb);
        }
        else {
            check_array_decl();
            try {
                tmp.type.elements = std::get<long> (ret_val.constant_value);
            }
            catch(const std::exception& e) {}
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
        else if( v == lex.STRUCT && current_token->next && current_token->next->token.code == lex.ID ) {
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

    type.symbol_name = "FUNC_ARG";
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

    if ( match(lex.SEMICOLON) || match(lex.LBRACKET) || match(lex.COMMA) ) {
        current_token = consumed_token;
        return 0;
    }

    if(!match(lex.LPAR)) {
        consumed_token = current_token;
        return 0;
    }

    symbol_table.add_symbol(sym);
    current_func = sym.name;
    // !!! BE CAREFUL with this
    current_depth++;

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


    if(!stm_block()) {
        std::cout << logger << utils::log_error(current_token->token.line, "Creating a function without a body ");
        exit(-1);
    }

    // !!! BE CAREFUL with this
    current_func = "";
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
        if ( expr() && cnt == 1) {  // check only for middle thing
            check_r_if();
        }

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

void SyntacticAnalyzer::check_r_if() {
    if (ret_val.type.type_base == ret_val.type.TB_STRUCT) {
        std::string s = "A [STRUCT] can't be [LOGICALLY TESTED]!";
        std::cout << logger << utils::log_error(current_token->token.line, s);
        exit(lex.ID);
    }
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

    check_r_if();

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
        auto type = type_base_condition();
        if(!type_name()) {
            if(expr()) {
                if(match(lex.RPAR)) {
                    std::cout << logger << "Found a PRIMARY ( LPAR EXPR RPAR)!\n";
                    return 1;
                }
            }
        }
        else if( match(lex.RPAR) ) {
            check_cast(type);
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
    int size = vec.size();

    if(match(lex.LPAR) ) {
        if(size) {
            if( it >= size ) {
                std::cout << logger << utils::log_error(current_token->token.line, "Too many arguments in function1!");
                exit(lex.LPAR);
            }
            std::cout << logger << "[RET_VAL]: " << utils::type_to_string(vec[it].second.type.type_base) << "\n";
            cast_type(vec[it].second.type, vec[it].second.type);
            ++it;

            expr(); // NOTE: EXPR() ? [optional] [ , EXPR() ]* ...  
        }

        while(match(lex.COMMA) && expr()) {
            if(size) {
                if( it >= size ) {
                    std::cout << logger << utils::log_error(current_token->token.line, "Too many arguments in function2!");
                    exit(lex.LPAR);
                }
                cast_type(vec[it].second.type, vec[it].second.type);
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

Symbol SyntacticAnalyzer::find_val_in_members(const Symbol& symb, const std::string& symb_name) {
    std::cout << logger << "[FIND_VAL_IN_MEMBERS]: STRUCT/FUNCTION: " << symb.name 
        << " searched SYMBOL: " << symb_name << "\n";
    auto vec = symb.members;
    for(const auto& x: vec) {
        if (x.second.name == symb_name) {
            return x.second;
        } 
    }
    return Symbol{};
}

void SyntacticAnalyzer::check_postfix(const ReturnValue& rv) {
    if (current_token && current_token->token.code == lex.ID) {
        auto symbol = symbol_table.find_symbol(rv.type.symbol_name); // Search for struct variable
        std::cout << logger << "[STRUCT ARRAY] : " << ret_val.type.symbol_name << "\n";
        if ( symbol.name == "" ) {
            std::string s = "Couldn't find any STRUCT VAR declared with the name: [" 
                + rv.type.symbol_name + "]";
            std::cout << logger << utils::log_error(current_token->token.line, s);
            exit(lex.ID);
        }

        if ( rv.type.elements >= 0) {
            std::string s = "In order to access struct ARRAY, you should first access the [ARRAY ELEMENT]!";
            std::cout << logger << utils::log_error(current_token->token.line, s);
            exit(lex.ID);    
        }

        auto struct_ = symbol_table.find_symbol(symbol.type.symbol_name);   // Search after actual struct saved
        
        std::string name = std::get<std::string> (current_token->token.text);
        auto ok = find_val_in_members(struct_, name); // ok is the symbol found in the STRUCT (using its type)
        if ( ok.name == "" ) {
            std::string s = "This STRUCT [" + struct_.name + "] " + "doesn't have " + name + " as parameter ";
            std::cout << logger << utils::log_error(current_token->token.line, s);
            exit(lex.ID);
        }
        ret_val.type = ok.type;
        ret_val.is_left_value = true;
        ret_val.is_constant_value = false;
    }
}

int SyntacticAnalyzer::expr_postfix_bracket() {

    consumed_token = std::make_shared<Node> (*current_token);

    if(expr_primary()) {
        return 1;
    }

    static ReturnValue ret;
    static std::string name;
    if ( symbol_table.find_symbol(ret_val.type.symbol_name).name != "") {
        name = symbol_table.find_symbol(ret_val.type.symbol_name).name;
        ret = ret_val;
        ret.type.symbol_name = name;
    }
    ret.type.elements = ret_val.type.elements;

    if(!match(lex.LBRACKET)) {
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

        Type t = ret_val.type;

        int cnt = 0 ;
        while(expr()) {
            ++cnt;
        }

        if(t.type_base == t.TB_STRUCT && cnt == 1 && t.elements >= 0) {
            auto symb_name = t.symbol_name;
            t = create_type(t.TB_STRUCT, -1);
            t.symbol_name = symb_name;
        }
        else {
            t = create_type(t.TB_INT, -1);
        }
        // std::cout << logger << "RET_VAL: " << ret_val.type.type_base << "   T: " << t.type_base << "\n";
        // std::cout << logger << "RET_VAL: " << ret_val.type.elements << "   T: " << t.elements << "\n";
        // std::cout << logger << "RET_VAL: " << ret_val.type.symbol_name << "   T: " << t.symbol_name << "\n";

        // TO DO: THIS IS STILL A BUG!
        if (t.type_base == t.TB_INT) {
            cast_type(ret_val.type, t);
        }

        ret_val.type = t;
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

void SyntacticAnalyzer::check_unary_sub() {
    if (ret_val.type.elements >= 0) {
        std::cout << logger << utils::log_error(current_token->token.line, "[CHECK_UNARY_SUB] Unary [-] cannot be applied to an array!");
        exit(lex.SUB);
    }

    if (ret_val.type.type_base == ret_val.type.TB_STRUCT) {
        std::cout << logger << utils::log_error(current_token->token.line, "[CHECK_UNARY_SUB] Unary [-] cannot be applied to a TB_STRUCT!");
        exit(lex.SUB);
    }
}

void SyntacticAnalyzer::check_unary_not() {
    if (ret_val.type.type_base == ret_val.type.TB_STRUCT) {
        std::cout << logger << utils::log_error(current_token->token.line, "[CHECK_UNARY_NOT] Unary [!] cannot be applied to a TB_STRUCT!");
        exit(lex.SUB);
    }
    ret_val.type = create_type(ret_val.type.TB_INT, -1);
    ret_val.is_constant_value = false;
    ret_val.is_left_value = false;
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
        check_unary_sub();
        return 1;
    }
     if( match(lex.NOT) ) {
        if( !expr_unary()) {
            current_token = consumed_token;
            return 0;
        }
        check_unary_not();
        return 1;
    }
    
    current_token = consumed_token;
    return 0;
}

void SyntacticAnalyzer::check_cast(const Type& type) {
    cast_type(type, ret_val.type);
    ret_val.type = type;
    ret_val.is_constant_value = false;
    ret_val.is_left_value = false;
}

int SyntacticAnalyzer::expr_cast() {
    consumed_token = std::make_shared<Node> (*current_token);

    if(expr_unary()) {
        return 1;
    }

    auto type = type_base_condition();

    if(!match(lex.LPAR)) {
        return 0;
    }

    std::cout << logger << "[ASTA] castuiesc la: " << utils::type_to_string(type.type_base) << "\n";
    check_cast(type);

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

void SyntacticAnalyzer::check_mul(const ReturnValue& rv) {
    if (rv.type.elements >= 0 || ret_val.type.elements >= 0) {
        std::cout << logger << utils::log_error(current_token->token.line, "An [ARRAY] can't be [MULTIPLIED] or [DIVIDED]!");
        exit(lex.MUL);
    }

    if (rv.type.type_base == rv.type.TB_STRUCT || ret_val.type.type_base == rv.type.TB_STRUCT) {
        std::cout << logger << utils::log_error(current_token->token.line, "A [STRUCT] can't be [MULTIPLIED] or [DIVIDED]!");
        exit(lex.MUL);
    }
    cast_type(ret_val.type, rv.type);
    ret_val.type = get_arithmetic_type(rv.type, ret_val.type);
    ret_val.is_constant_value = false;
    ret_val.is_left_value = false;
}

int SyntacticAnalyzer::expr_mul_helper() {
    consumed_token = std::make_shared<Node> (*current_token);
    
    ReturnValue rv = ret_val;
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
    check_mul(rv);
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

void SyntacticAnalyzer::check_add(const ReturnValue& rv) {
    if (rv.type.elements >= 0 || ret_val.type.elements >= 0) {
        std::cout << logger << utils::log_error(current_token->token.line, "An [ARRAY] can't be [ADDED] or [SUBSTRACTED]!");
        exit(lex.MUL);
    }

    if (rv.type.type_base == rv.type.TB_STRUCT || ret_val.type.type_base == rv.type.TB_STRUCT) {
        std::cout << logger << utils::log_error(current_token->token.line, "A [STRUCT] can't be [ADDED] or [SUBSTRACTED]!");
        exit(lex.MUL);
    }
    cast_type(ret_val.type, rv.type);
    ret_val.type = get_arithmetic_type(rv.type, ret_val.type);
    ret_val.is_constant_value = false;
    ret_val.is_left_value = false;    
}

int SyntacticAnalyzer::expr_add_helper() {
    consumed_token = std::make_shared<Node> (*current_token);
    
    ReturnValue rv = ret_val;
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
    check_add(rv);
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

void SyntacticAnalyzer::check_rel(const ReturnValue& rv) {
    if (rv.type.elements >= 0 || ret_val.type.elements >= 0) {
        std::cout << logger << utils::log_error(current_token->token.line, "An [ARRAY] can't be [COMPARED]!");
        exit(lex.MUL);
    }

    if (rv.type.type_base == rv.type.TB_STRUCT || ret_val.type.type_base == rv.type.TB_STRUCT) {
        std::cout << logger << utils::log_error(current_token->token.line, "A [STRUCT] can't be [COMPARED]!");
        exit(lex.MUL);
    }
    ret_val.type = create_type(ret_val.type.TB_INT, -1);
    ret_val.is_constant_value = false;
    ret_val.is_left_value = false;        
}

int SyntacticAnalyzer::expr_rel_helper() {
    consumed_token = std::make_shared<Node> (*current_token);
    
    ReturnValue rv = ret_val;
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
    check_rel(rv);
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

void SyntacticAnalyzer::check_eq(const ReturnValue& rv) {
    if (rv.type.elements >= 0 || ret_val.type.elements >= 0) {
        std::cout << logger << utils::log_error(current_token->token.line, "An [ARRAY] can't be [COMPARED]!");
        exit(lex.MUL);
    }

    if (rv.type.type_base == rv.type.TB_STRUCT || ret_val.type.type_base == rv.type.TB_STRUCT) {
        std::cout << logger << utils::log_error(current_token->token.line, "A [STRUCT] can't be [COMPARED]!");
        exit(lex.MUL);
    }
    ret_val.type = create_type(ret_val.type.TB_INT, -1);
    ret_val.is_constant_value = false;
    ret_val.is_left_value = false;         
}

int SyntacticAnalyzer::expr_eq_helper() {
    consumed_token = std::make_shared<Node> (*current_token);
    
    ReturnValue rv = ret_val;
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
    check_eq(rv);

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

void SyntacticAnalyzer::check_and(const ReturnValue& rv) {
    if (rv.type.type_base == rv.type.TB_STRUCT || ret_val.type.type_base == rv.type.TB_STRUCT) {
        std::cout << logger << utils::log_error(current_token->token.line, "A [STRUCT] can't be [LOGICALLY TESTED]!");
        exit(lex.MUL);
    }
    ret_val.type = create_type(ret_val.type.TB_INT, -1);
    ret_val.is_constant_value = false;
    ret_val.is_left_value = false;      
}

int SyntacticAnalyzer::expr_and_helper() {
    consumed_token = std::make_shared<Node> (*current_token);
    
    ReturnValue rv = ret_val;
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
    check_and(rv);
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

void SyntacticAnalyzer::check_or(const ReturnValue& rv) {
    if (rv.type.type_base == rv.type.TB_STRUCT || ret_val.type.type_base == rv.type.TB_STRUCT) {
        std::cout << logger << utils::log_error(current_token->token.line, "A [STRUCT] can't be [LOGICALLY TESTED]!");
        exit(lex.MUL);
    }
    ret_val.type = create_type(ret_val.type.TB_INT, -1);
    ret_val.is_constant_value = false;
    ret_val.is_left_value = false;      
}

int SyntacticAnalyzer::expr_or_helper() {
    consumed_token = std::make_shared<Node> (*current_token);
    
    ReturnValue rv = ret_val;
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
    check_or(rv);

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

void SyntacticAnalyzer::check_assign(const ReturnValue& rv) {
    if ( !rv.is_left_value ) {
        std::string s = "Can't [ASSIGN] to [NON-LVAL]!";
        std::cout << logger << utils::log_error(current_token->token.line , s);
        exit(lex.ID);
    }

    if (rv.type.elements >= 0|| ret_val.type.elements >= 0) {
        std::cout << logger << utils::log_error(current_token->token.line, "Arrays can't be [ASSIGNED]!");
        exit(lex.ID);
    }
    cast_type(ret_val.type, rv.type);
    ret_val.is_constant_value = false;
    ret_val.is_left_value = false;      
}

int SyntacticAnalyzer::expr_assign_helper() {
    consumed_token = std::make_shared<Node> (*current_token);
    
    ReturnValue rv = ret_val;
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
    check_assign(rv);
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
    std::cout << logger << "[CAST_TYPE] intre [" << utils::type_to_string(dest.type_base) 
        << "] si [" << utils::type_to_string(src.type_base) << "]\n";
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
        case 4: // TB_STRUCT
            if(dest.type_base == src.TB_STRUCT ) {
                // std::cout << logger << "[" << src.symbol_name << "] and dest: [" << dest.symbol_name << "]\n";
                // TO DO: Modify this to correspond in future
                if(src.symbol_name != dest.symbol_name) {
                    std::cout << logger << utils::log_error(current_token->token.line, "A struct cannot be converted to another struct!");
                    exit(lex.ID);         
                }
                return;
            }
    }

    std::cout << logger << utils::log_error(current_token->token.line, "Incompatible types!");
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

    std::cout << logger << utils::log_error(current_token->token.line, "Couldn't get Arithmetic Type!");
    exit(lex.ID);
}

Type SyntacticAnalyzer::create_type(const int& type_base, const int& elements) {
    Type t;
    t.type_base = type_base;
    t.elements = elements;
    return t;
}

void SyntacticAnalyzer::add_ext_func(const std::string &str, 
        std::vector<Symbol> arr, 
        const int& type) {

    auto res = tmp.create_function(str, arr, type);
    if ( str == "put_s") {
        res.addr_offset = reinterpret_cast<void *>(SyntacticAnalyzer::put_s);
        // auto val = std::get<void * > (res.addr_offset);
        // std::cout << logger << static_cast<void *> (&val) << "\n";
    }
    else if (str == "get_s") {
        res.addr_offset = reinterpret_cast<void *> (SyntacticAnalyzer::get_s);
    }
    else if (str == "put_i") {
        res.addr_offset = reinterpret_cast<void *> (SyntacticAnalyzer::put_i);
    }
    else if( str == "get_i") {
        res.addr_offset = reinterpret_cast<void *> (SyntacticAnalyzer::get_i);
    }
    else if (str == "put_d") {
        res.addr_offset = reinterpret_cast<void *> (SyntacticAnalyzer::put_d);
    }
    else if( str == "get_d") {
        res.addr_offset = reinterpret_cast<void *> (SyntacticAnalyzer::get_d);
    }
    else if ( str == "put_c") {
        res.addr_offset = reinterpret_cast<void *> (SyntacticAnalyzer::put_c);
    }
    else if ( str == "get_c") {
        res.addr_offset = reinterpret_cast<void *> (SyntacticAnalyzer::get_c);
    }
    else if ( str == "seconds") {
        res.addr_offset = reinterpret_cast<void *> (SyntacticAnalyzer::seconds);
    }
    symbol_table.add_symbol(res);
}

void SyntacticAnalyzer::add_predefined_functions() {
    // void put_s()
    Type type{ret_val.type.TB_CHAR};
    type.elements = 0;
    Symbol s = Symbol("s", tmp.CLS_VAR, type);
    add_ext_func("put_s", {s}, ret_val.type.TB_VOID);

    // void get_s(char s[])
    type = Type{ret_val.type.TB_CHAR};
    type.elements = 0;
    s = Symbol("s", tmp.CLS_VAR, type);
    add_ext_func("get_s", {s}, ret_val.type.TB_VOID);

    // void put_i(int i)
    type = Type{ret_val.type.TB_INT};
    s = Symbol("i", tmp.CLS_VAR, type);
    add_ext_func("put_i", {s}, ret_val.type.TB_VOID);

    // int get_i()
    add_ext_func("get_i", {}, ret_val.type.TB_INT);

    // void put_d(double d)
    type = Type{ret_val.type.TB_DOUBLE};
    s = Symbol("d", tmp.CLS_VAR, type);
    add_ext_func("put_d", {s}, ret_val.type.TB_VOID);

    // double get_d()
    add_ext_func("get_d", {s}, ret_val.type.TB_DOUBLE);

    // void put_c(char c)
    type = Type{ret_val.type.TB_CHAR};
    s = Symbol("c", tmp.CLS_VAR, type);
    add_ext_func("put_c", {s}, ret_val.type.TB_VOID);

    // char get_c()
    add_ext_func("get_c", {}, ret_val.type.TB_CHAR);

    // double seconds()
    add_ext_func("seconds", {}, ret_val.type.TB_DOUBLE);
}

void SyntacticAnalyzer::put_s() {
    std::cout << " [PUT_S] " << (int)vm.pop_c() << "\n";
}

void SyntacticAnalyzer::put_i() {
    std::cout << " [PUT_I] " << vm.pop_i() << "\n";
}

void SyntacticAnalyzer::put_c() {
    std::cout << " [PUT_C] " << vm.pop_c() << "\n";
}

void SyntacticAnalyzer::put_d() {
    std::cout << " [PUT_D] " << vm.pop_d() << "\n";
}

void SyntacticAnalyzer::get_i() {
    std::cout << " [GET_I] " << 5 << "\n";
}

void SyntacticAnalyzer::get_d() {
    std::cout << " [GET_D] " << 5<< "\n";
}

void SyntacticAnalyzer::get_s() {
    std::cout << " [GET_S] " << 5 << "\n";
}

void SyntacticAnalyzer::get_c() {
    std::cout << " [GET_C] " << 5 << "\n";
}

void SyntacticAnalyzer::seconds() {
    std::cout << " [SECONDS] " << time(0) << "\n";
}

void SyntacticAnalyzer::test_mv() {
    long * v = static_cast<long * > (vm.alloc_heap(sizeof(long)));
    Instruction h;
    InstructionList il;

    h = Instruction{h.O_PUSHCT_A};
    h.set_args({v});
    il.insert_instr(h);

    h = Instruction{h.O_PUSHCT_I};
    h.set_args({static_cast<long> (3)});
    il.insert_instr(h);

    h = Instruction{h.O_STORE};
    long val = sizeof(long);
    h.set_args({val});
    il.insert_instr(h);

    h = Instruction{h.O_PUSHCT_A};
    h.set_args({v});
    il.insert_instr(h);

    h = Instruction{h.O_LOAD};
    h.set_args({val});
    il.insert_instr(h);

    // h = Instruction{h.O_CALLEXT};
    // if ( Instruction::variant_to_type(symbol_table.symbol_exists("put_i").addr_offset) == "void" ) {
    //     h.set_args({std::get<void *> (symbol_table.find_symbol("put_i").addr_offset)});
    //     il.insert_instr(h);
    // }

    h = Instruction{h.O_PUSHCT_I};
    h.set_args({static_cast<long> (3)});
    il.insert_instr(h);

    h = Instruction{h.O_PUSHCT_I};
    h.set_args({static_cast<long> (0)});
    il.insert_instr(h);

    auto j = Instruction{h.O_HALT};
    h = Instruction{h.O_JMP};
    h.set_args({reinterpret_cast<void *>(&j)});
    il.insert_instr(h);

    h = Instruction{h.O_PUSHCT_A};
    h.set_args({v});
    il.insert_instr(h);
    il.insert_instr(h);

    h = Instruction{h.O_LOAD};
    h.set_args({val});
    il.insert_instr(h);

    h = Instruction{h.O_PUSHCT_I};
    h.set_args({static_cast<long> (1)});
    il.insert_instr(h);
    il.insert_instr(h);

    il.insert_instr(Instruction{h.O_ADD_I});

    h = Instruction{h.O_PUSHCT_I};
    h.set_args({static_cast<long> (15)});
    il.insert_instr(h);

    il.insert_instr(Instruction{h.O_SUB_I});
    
    h = Instruction{h.O_HALT};
    il.insert_instr(h);

    vm.set_il(il);
    vm.run();
}