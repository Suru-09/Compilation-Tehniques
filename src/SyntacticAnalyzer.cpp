#include "SyntacticAnalyzer.hpp"

SyntacticAnalyzer::SyntacticAnalyzer(const LexicalAnalyzer& lex_analiz) 
: lex(lex_analiz),
class_name("SyntacticAnalyzer")
{   
    current_token = lex.token_list.get_head();
    logger = Logger{class_name};
}

SyntacticAnalyzer::SyntacticAnalyzer() 
: class_name("SyntacticAnalyzer")
{   
    logger = Logger{class_name};
}

int SyntacticAnalyzer::term() {
    if(current_token->token.code == lex.CT_INT) {
        std::cout << logger << " ----> ";
        current_token->token.print_text();
        match(lex.CT_INT);
        return 1;
    }
    else if(current_token->token.code == lex.CT_REAL) {
        std::cout << logger << " ----> ";
        current_token->token.print_text();
        match(lex.CT_REAL);
        return 1;
    }

    return 0;
}

int SyntacticAnalyzer::expr() {

    consumed_token = std::make_shared<Node> (*current_token);
    std::cout << logger << "eu sunt EXPR: " << lex.print_pretty(current_token->token.code) << "\n";

   
    // if(expr_primary()) {
    //     std::cout << logger << "Am gasit expresie primara!\n";
    //     return 1;
    // }
    // else if(expr_postfix() ) {
    //     std::cout << logger << "Am gasit expresie POSTFIX!\n";
    //     return 1;
    // }
    // else if(expr_unary()) {
    //     std::cout << logger << "Am gasit expresie UNARY!\n";
    //     return 1;
    // }
    // else if(expr_cast()) {
    //     std::cout << logger << "Am gasit expresie CAST!\n";
    //     return 1;
    // }
    if(expr_mul()) {
        std::cout << logger << "Am gasit expresie MUL!\n";
        return 1;
    }
    else {
        current_token = consumed_token;
        return 0;
    }
}

int SyntacticAnalyzer::match(const int& code) {
    // std::cout << logger << "Given code: " << code << "\n";
    // std::cout << logger << "Actual code: " << current_token->token.code << "\n";
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
        std::cout << logger << "Found EXPR statement!\n";
        return 1;        
    }
    else if(stm_block() == 1) {
        std::cout << logger << "Am gasit while in LACC () RACC!\n";
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
        // std::cout << logger << utils::log_error(current_token->token.line, "Missing { ");
        return 0;
    }

   while( stm() ||  decl_struct() || decl_var() ) {
    //    std::cout << logger << "STM_BLOCK a ramas in bucla infinita!\n";
   }

    if(!match(lex.RACC)) {
        std::cout << logger << utils::log_error(current_token->token.line, "Missing } ");
        exit(lex.RACC);
    }

    return 1;
}

void SyntacticAnalyzer::unit() {
    while(1) {
        if(current_token->token.code == lex.END) {
            break;
        }

        if ( decl_struct() == 1) {
            // std::cout << logger << "Found a STRUCT!\n";
        }
        else if ( decl_var() == 1) {
            // std::cout << logger << "Found a VARIABLE!\n";
        }
        else if ( decl_func() == 1) {
            // std::cout << logger << "Found a function!\n";
        }

        // std::cout << logger << "Bucla infinita in UNIT!\n";
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
        return 0;
    }

    expr();

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

int SyntacticAnalyzer::decl_var() {
    consumed_token = std::make_shared<Node>(*current_token);

    if(!type_base() ) {
        return 0;
    }

    if(match(lex.MUL)) {
        current_token = consumed_token;
        return 0;
    }

    if(!match(lex.ID)) {
        std::cout << logger << utils::log_error(current_token->token.line, "Missing ID ");
        exit(lex.ID);
    }

    array_decl();

    while(1) {
        if(!match(lex.COMMA)) {
            // std::cout << logger << utils::log_error(current_token->token.line, "Missing COMMA ");
            break;
        }

        if(!match(lex.ID)) {
            std::cout << logger << utils::log_error(current_token->token.line, "Missing ID ");
            exit(lex.ID);
        }

        array_decl();
    }

    if(match(lex.LPAR)) {
        current_token = consumed_token;
        // std::cout << logger << "Am intrat pe LPAR2: " << lex.print_pretty(consumed_token->token.code) << "\n";
        return 0;
    }

    if(!match(lex.SEMICOLON)) {
        std::cout << logger << utils::log_error(current_token->token.line, "Missing SEMICOLON ");
        exit(lex.SEMICOLON);
    }

    std::cout << logger << "Found a VARIABLE!\n";
    return 1;
}

int SyntacticAnalyzer::decl_struct() {
    consumed_token = std::make_shared<Node>(*current_token);

    if(!match(lex.STRUCT) || !match(lex.ID)) {
        current_token = consumed_token;
        return 0;
    }

    if(!match(lex.LACC)) {
        std::cout << logger << utils::log_error(current_token->token.line, "Missing { ");
        exit(lex.LACC);
    }

    while( decl_var() ) {}

    if(!match(lex.RACC)) {
        std::cout << logger << utils::log_error(current_token->token.line, "Missing { ");
        exit(lex.RACC);
    }

    if(!match(lex.SEMICOLON)) {
        std::cout << logger << utils::log_error(current_token->token.line, "Missing SEMICOLON ");
        exit(lex.SEMICOLON);
    }

    std::cout << logger << "Found a STRUCT!\n";
    return 1;
}

int SyntacticAnalyzer::func_arg() {

    if(!type_base()) {
        return 0;
    }

    if(!match(lex.ID)) {
        std::cout << logger << utils::log_error(current_token->token.line, "Missing ID ");
        exit(lex.ID);
    }

    array_decl();

    return 1;
}

int SyntacticAnalyzer::decl_func() {

    // std::cout << logger << "Sunt in DECL_FUNC: " <<  lex.print_pretty(current_token->token.code) << " at line: " << current_token->token.line << "\n";

    if( type_base()) {
        match(lex.MUL);
    }
    else if(!match(lex.VOID)) {
        return 0;
    }

    if(!match(lex.ID)) {
        std::cout << logger << utils::log_error(current_token->token.line, "Missing ID ");
        exit(lex.ID);
    }

    if(!match(lex.LPAR)) {
        std::cout << logger << utils::log_error(current_token->token.line, "Missing ( ");
        exit(lex.LPAR);
    }

    while(1) {
        if(func_arg() == 1) {
            if(!match(lex.COMMA) && !func_arg())
                break;

            if(match(lex.COMMA) && !func_arg()) {
                std::cout << logger << utils::log_error(current_token->token.line, "Missing COMMA ");
                exit(lex.COMMA);
            }

            if(!match(lex.COMMA) && func_arg()) {
                std::cout << logger << utils::log_error(current_token->token.line, "Missing arguments ");
                exit(-1);
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
        std::cout << logger << "  " << cnt << '\n';
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

    std::cout << logger << "Sunt in expression R_IF: " << lex.print_pretty(current_token->token.code) << "\n";

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

    expr();

    if(!match(lex.SEMICOLON)) {
        return 0;   
    }

    return 1;
}

int SyntacticAnalyzer::expr_primary() {
    consumed_token = std::make_shared<Node> (*current_token);

    if(match(lex.CT_INT)) {
        return 1;
    }
    else if(match(lex.CT_REAL)){
        return 1;
    }
    else if(match(lex.CT_CHAR)) {
        return 1;
    }
    else if(match(lex.CT_STRING)) {
        return 1;
    }
    else if(match(lex.LPAR)){
        if(!type_name()) {
            if(expr()) {
                if(match(lex.RPAR)) {
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
            if(expr()) {
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
            }
            else {
                std::cout << logger << utils::log_error(current_token->token.line, "Missing EXPRESSION ");
                exit(-1);
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

    return 1;
}

int SyntacticAnalyzer::expr_postfix_bracket() {

    consumed_token = std::make_shared<Node> (*current_token);

    if(expr_primary()) {
        std::cout << logger << "Found a POSTFIX!\n";
        return 1;
    }

    if(!match(lex.LBRACKET)) {
        if(!match(lex.DOT)) {
            return 0;
        }

        if(!match(lex.ID)) {
            return 0;
        }
    }
    else {
        while(expr_postfix_bracket()) {}

        if(!match(lex.RBRACKET)) {
            current_token = consumed_token;
            return 0;
        }
    }

    return 1;
}

int SyntacticAnalyzer::expr_postfix() {
   
   consumed_token = std::make_shared<Node> (*current_token);

   if(!expr_postfix_bracket()){
       current_token = consumed_token;
       return 0;
   }
    
    std::cout << logger << "Found a POSTFIX!\n";
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
        // current_token = consumed_token;
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

    std::cout << logger << "Sunt in expression MUL HELPER: " << lex.print_pretty(current_token->token.code) << "\n";
    
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

    std::cout << logger << "Sunt in expression MUL HELPER FINAL: " << lex.print_pretty(current_token->token.code) << "\n";
    return 1;
}


int SyntacticAnalyzer::expr_mul() {
    
    std::cout << logger << "Sunt in expression MUL: " << lex.print_pretty(current_token->token.code) << "\n";
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


