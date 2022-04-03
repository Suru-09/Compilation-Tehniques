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
    // term();
    while(true) {
        if(expr_primary()) {
            std::cout << logger << "Am gasit expresie primara!\n";
            return 1;
        }
        if(expr_postfix() ) {
            std::cout << logger << "Am gasit expresie POSTFIX!\n";
            return 1;
        }
        // if(expr_unary()) {
        //     std::cout << logger << "Am gasit expresie UNARY!\n";
        //     return 1;
        // }
        // if(expr_cast()) {
        //     std::cout << logger << "Am gasit expresie CAST!\n";
        //     return 1;
        // }
        // if(expr_mul()) {
        //     std::cout << logger << "Am gasit expresie MUL!\n";
        //     return 1;
        // }
        else {
            return 0;
        }
        
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
    // std::cout << logger << "Am intrat in TYPE_BASE : " << lex.print_pretty(consumed_token->token.code) << "\n";

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
            // std::cout << logger << "Am intrat in STRUCT : " << lex.print_pretty(consumed_token->token.code) << "\n";
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
    // std::cout << logger << "Am intrat in DECL_VAR : " << lex.print_pretty(consumed_token->token.code) << "\n";

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
    if(!match(lex.STRUCT) || !match(lex.ID)) {
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
    else if(match(lex.LPAR) && expr() && match(lex.RPAR)){
        return 1;
    }


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

    return 1;
}

int SyntacticAnalyzer::expr_postfix_bracket() {

    if(expr_primary()) {
        return 1;
    }


    if(!expr_postfix()) {
        return 0;
    }
    
    if(!match(lex.LBRACKET)) {
       if(!match(lex.DOT)) {
           std::cout << logger << utils::log_error(current_token->token.line, "Missing both [ OR DOT ");
           exit(lex.DOT);
       }

       if(!match(lex.ID)) {
           std::cout << logger << utils::log_error(current_token->token.line, "Missing ID after DOT");
           exit(lex.ID);
       }
    }

    if(!expr()) {
        std::cout << logger << utils::log_error(current_token->token.line, "Missing expression ");
        exit(-1);
    }

    return 1;
}

int SyntacticAnalyzer::expr_postfix() {

    std::cout << logger << "Loop infinit ? !\n";

    if(expr_postfix_bracket() == 1) {
        return 1;
    }

    if(!expr_primary()) {
        return 0;
    }

    return 1;
}

int SyntacticAnalyzer::expr_unary() {
    if(!match(lex.SUB) && !match(lex.NOT)) {
        if(!(expr_postfix())) {
            return 0;
        }
    }
    else if(match(lex.SUB)) {
        if(!expr_unary()) {
            return 0;
        }
    }
    else if(match(lex.NOT)) {
        if(!expr_unary()) {
            return 0;
        }
    }

    return 1;
}

int SyntacticAnalyzer::expr_cast() {

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

int SyntacticAnalyzer::expr_mul() {
    if(expr_cast()) {
        return 1;
    }

    if(expr_mul()) {
        if(!match(lex.MUL) && !match(lex.DIV)) {
            std::cout << logger << utils::log_error(current_token->token.line, "Missing both MUL and DIV ");
            exit(lex.MUL);
        }
        else if(match(lex.MUL)) {
            if(!expr_cast()) {
                std::cout << logger << utils::log_error(current_token->token.line, "Missing EXPR_CAST ");
                exit(lex.MUL);
            }
        }
        else if(match(lex.DIV)) {
            if(!expr_cast()) {
                std::cout << logger << utils::log_error(current_token->token.line, "Missing EXPR_CAST ");
                exit(lex.MUL);
            }
        }
        else {
            return 1;
        }
    }

    return 0;
}


