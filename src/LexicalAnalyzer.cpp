#include "LexicalAnalyzer.hpp"
#include "Token.hpp"
#include "TokenList.hpp"

int LexicalAnalyzer::get_next_token() {
    int state = 0, nCh;
    char ch;
    const char *pStartCh;
    Token *tk;

    while(1){ // infinite loop
        ch=*pCrtCh;

		//switch cu toate starile din diagrama
		printf("#%d %c(%d)\n", s, *pch, *pch); // testare/debbuging
		switch (s){
		case(0) :
			if (*pch == ' ' || *pch == '\t' || *pch == '\r' || *pch == '\n'){
				if (*pch == '\n') {
					line++;
				}
				pch++;
			}
			else if (isalpha(*pch) || *pch == '_'){
				s = 1;
				start = pch++;
			}
			else if (*pch == '='){
				s = 3;
				pch++;
			}
			else if (*pch == ','){
				s = 6;
				pch++;
			}
			else if (*pch == ';'){
				s = 7;
				pch++;
			}
			else if (*pch == '('){
				s = 8;
				pch++;
			}
			else if (*pch == ')'){
				s = 9;
				pch++; 
			}
			else if (*pch == '['){
				s = 10;
				pch++;
			}
			else if (*pch == ']'){
				s = 11;
				pch++;
			}
			else if (*pch == '{'){
				s = 12;
				pch++;
			}
			else if (*pch == '}'){
				s = 13;
				pch++;
			}
			else if (*pch == '\0'){
				addTk(END);
				return END;
			}
			else if (*pch == '+'){
				s = 15;
				pch++;
			}
			else if (*pch == '-'){
				s = 16;
				pch++;
			}
			else if (*pch == '*'){
				s = 17;
				pch++;
			}
			else if (*pch == '/'){
				s = 18;
				pch++;
			}
			else if (*pch == '.'){
				s = 21;
				pch++;
			}
			else if (*pch == '&'){
				s = 22;
				pch++;
			}
			else if (*pch == '|'){
				s = 24;
				pch++;
			}
			else if (*pch == '!'){
				s = 26;
				pch++;
			}
			else if (*pch == '<'){
				s = 29;
				pch++;
			}
			else if (*pch == '>'){
				s = 32;
				pch++;
			}
			else if (isdigit(*pch)){
				s = 35;
				pch++;
			}
			else if (*pch == '\''){
				s = 43;
				pch++;
			}
			else if (*pch == '"'){
				s = 46;
				pch++;
			}
			break;
		case(1) :
			if (isalnum(*pch) || *pch == '_'){
				pch++;
			}
			else{
				s = 2; // nu consuma nimic 
			}
			break;
		case(2) :
			//pch pe primul caract de dupa ID => ID = [start, pch)
			tk = addTk(ID);
			tk->text = extract(start, pch);
			if (strcmp("break", tk->text) == 0){
				tk->code = BREAK;
			}
			else if (strcmp("char", tk->next) == 0){
				tk->code = CHAR;
			}
			else if (strcmp("double", tk->next) == 0){
				tk->code = DOUBLE;
			}
			else if (strcmp("else", tk->next) == 0){
				tk->code = ELSE;
			}
			else if (strcmp("for", tk->next) == 0){
				tk->code = FOR;
			}
			else if (strcmp("if", tk->next) == 0){
				tk->code = IF;
			}
			else if (strcmp("int", tk->next) == 0){
				tk->code = INT;
			}
			else if (strcmp("return", tk->next) == 0){
				tk->code = RETURN;
			}
			else if (strcmp("struct", tk->next) == 0){
				tk->code = STRUCT;
			}
			else if (strcmp("void", tk->next) == 0){
				tk->code = VOID;
			}
			else if (strcmp("while", tk->next) == 0){
				tk->code = WHILE;
			}
			return tk->code;
			break;
		case(3) :
			if (*pch == '='){
				s = 5;
				pch++;
			}
			else{
				s = 4;
			}
			break;
		case(4) :
			addTk(ASSIGN);
			return ASSIGN;
			break;
		case(5) :
			addTk(EQUAL);
			return EQUAL;
			break;
		default:
			printf("Stare inexistenta: %d (ch = %c)\n", s, *pch);
		}
	}
}