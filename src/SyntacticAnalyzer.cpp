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

void SyntacticAnalyzer::term() {
    if(current_token->token.code == lex.CT_INT) {
        std::cout << logger << " ----> ";
        current_token->token.print_text();
        match(lex.CT_INT);
    }
    else if(current_token->token.code == lex.CT_REAL) {
        std::cout << logger << " ----> ";
        current_token->token.print_text();
        match(lex.CT_REAL);
    }
    else {
        std::cout << logger << "The term read is not a CT_INT nor a CT_REAL!!\n";
        exit(1);
    }
}

int SyntacticAnalyzer::expr() {
    term();
    while(true) {
        if(current_token->token.code == lex.ADD) {
            match(lex.ADD);
            term();
            std::cout << logger << " + \n";
        }
        else if(current_token->token.code == lex.SUB) {
            match(lex.SUB);
            term();
            std::cout << logger << " - \n";
        }
        else if(current_token->token.code == lex.DIV) {
            match(lex.DIV);
            term();
            std::cout << logger << " - \n";
        }
        else if(current_token->token.code == lex.MUL) {
            match(lex.MUL);
            term();
            std::cout << logger << " - \n";
        }
        else {
            return 1;
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

    if(stm_block() == 1) {
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

    return 0;
}

int SyntacticAnalyzer::stm_block() {
    if(!match(lex.LACC)) {
        // std::cout << logger << utils::log_error(current_token->token.line, "Missing { ");
        return 0;
    }

   while( stm() ||  decl_struct() || decl_var() ) {}

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
        std::cout << logger << utils::log_error(current_token->token.line, "Missing identifier at function declation ");
        exit(lex.VOID);
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

