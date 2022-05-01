#include "SyntacticAnalyzer.hpp"

SyntacticAnalyzer::SyntacticAnalyzer(const LexicalAnalyzer& lex_analiz) 
: lex(lex_analiz),
class_name("SyntacticAnalyzer"),
current_depth(0),
current_struct(""),
current_func("")
{   
    current_token = lex.token_list.get_head();
    logger = Logger{class_name};
}

SyntacticAnalyzer::SyntacticAnalyzer() 
: class_name("SyntacticAnalyzer"),
current_depth(0),
current_struct(""),
current_func("")
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
    if(current_token->token.code == code) {
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
            add_var(tmp.type, tmp.name);
        }
        return 0;
    }

    expr();
    if(tmp.name != "" && symbol_table.find_symbol(tmp.name).name == "") {
        tmp.type.elements = 1;
        add_var(tmp.type, tmp.name);
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
            std::cout << logger << "Inainte sa ies zic cine sunt: " << sym.name << "\n";
            symbol_table.print_symbol_table();
            std::cout << logger << utils::log_error(current_token->token.line, "Symbol redefiniton ");
            exit(lex.ID);
        }
        sym.name = possible_key;
        sym.class_ = sym.CLS_STRUCT;
        sym.depth = current_depth;
        sym.type = type;
        // TO DO: set members in struct
        current_struct = possible_key;
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

   if(!type_base()) {
        current_token = consumed_token;
        return 0;
   }

   if(match(lex.ID)) {
        current_token = consumed_token;
        return 0;
   }

    if(!match(lex.LACC)) {
        current_token = consumed_token;
        if(!decl_var()) {
            std::cout << logger << utils::log_error(current_token->token.line, "Missing { ");
            exit(lex.LACC);
        }
        return 0;
    }

    while( decl_var() ) {}

    if(!match(lex.RACC)) {
        std::cout << logger << utils::log_error(current_token->token.line, "Missing } ");
        exit(lex.RACC);
    }

    if(!match(lex.SEMICOLON)) {
        std::cout << logger << utils::log_error(current_token->token.line, "Missing SEMICOLON ");
        exit(lex.SEMICOLON);
    }

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

                sym.add_member(name);
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

int SyntacticAnalyzer::expr_primary() {
    consumed_token = std::make_shared<Node> (*current_token);

    if(match(lex.CT_INT)) {
        std::cout << logger << "Found a PRIMARY (CT_INT) !\n";
        return 1;
    }
    else if(match(lex.CT_REAL)){
        std::cout << logger << "Found a PRIMARY (CT_REAL) \n";
        return 1;
    }
    else if(match(lex.CT_CHAR)) {
        std::cout << logger << "Found a PRIMARY (CT_CHAR) !\n";
        return 1;
    }
    else if(match(lex.CT_STRING)) {
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

    if(!match(lex.ID)) {
        return 0;
    }

    if(match(lex.LPAR) ) {
            
        expr();

        while(match(lex.COMMA) && expr()) {}

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

int SyntacticAnalyzer::expr_postfix_bracket() {

    consumed_token = std::make_shared<Node> (*current_token);

    if(expr_primary()) {
        return 1;
    }

    if(!match(lex.LBRACKET)) {
        
        if(!match(lex.DOT)) {
            current_token = consumed_token;
            return 0;
        }

        if(!match(lex.ID)) {
            current_token = consumed_token;
            return 0;
        }

        expr_postfix_bracket();
    }
    else {
        int cnt = 0 ;

        while(expr()) {
            ++cnt;
        }

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
        sym = symbol_table.find_symbol(current_struct);
        if( sym.class_name == "") {
            sym.name = name;
            sym.class_  = sym.CLS_VAR;
            sym.type = type;
            sym.depth = current_depth;
            symbol_table.add_symbol(sym);
            return true;
        }
        else {
            std::cout << logger << utils::log_error(current_token->token.line, "2Symbol redefinition while adding ");
            return false;
        }
    }
    else if( current_func != "" ) {
        sym = symbol_table.find_symbol(current_struct);
        if( sym.class_name != "" && sym.depth == current_depth ) {
            std::cout << logger << utils::log_error(current_token->token.line, "3Symbol redefinition while adding ");
            return false;
        }
        sym.name = name;
        sym.class_  = sym.CLS_VAR;
        sym.memory_zone = sym.MEM_LOCAL;
        sym.depth = current_depth;
        sym.type = type;
        symbol_table.add_symbol(sym);
    }
    else {
        sym = symbol_table.find_symbol(current_struct);
        if( sym.class_name != "") {
            std::cout << logger << utils::log_error(current_token->token.line, "4Symbol redefinition while adding ");
            return false;
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