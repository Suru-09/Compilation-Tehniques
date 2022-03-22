#include "LexicalAnalyzer.hpp"
#include "Token.hpp"
#include "TokenList.hpp"


#include <string>


int LexicalAnalyzer::get_next_token() {
    int state = 0, start = 0;
	int count = 0;

    while(1){
		count++;
		// if(count > 15)
		// 	return -1;

		if(i > given_text.size() - 1)
			return -1;

		std::cout << "I am i: " << i << " size: " << given_text.size() - 1 << "\n";
		std::cout << "I am state : " << state << " with ch = (" << given_text[i] << ")\n";
		switch (state){
			case(0):
				if (given_text[i] == ' ' || given_text[i] == '\t' ||
					given_text[i] == '\r' || given_text[i] == '\n'){
					
					if (given_text[i] == '\n') {
						line++;
					}
					i++;
				}
				else if(given_text[i] == ',') {
					i++;
					return COMMA;
				}
				else if(given_text[i] == ';') {
					i++;
					return SEMICOLON;
				}
				else if(given_text[i] == '(') {
					i++;
					return LPAR;
				}
				else if(given_text[i] == ')') {
					i++;
					return RPAR;
				}
				else if(given_text[i] == '[') {
					i++;
					return LBRACKET;
				}
				else if(given_text[i] == ']') {
					i++;
					return RBRACKET;
				}
				else if(given_text[i] == '{') {
					i++;
					return LACC;
				}
				else if(given_text[i] == '}') {
					i++;
					return RACC;
				}
				else if(given_text[i] == '+') {
					i++;
					return ADD;
				}
				else if(given_text[i] == '-') {
					i++;
					return SUB;
				}
				else if(given_text[i] == '*') {
					i++;
					return MUL;
				}
				else if(given_text[i] == '/') {
					i++;
					if(given_text[i] == '/') {
						i++;
						state = 28;
					}
				}
				else if(given_text[i] == '.') {
					i++;
					return DOT;
				}
				else if(given_text[i] == '&') {
					i++;
					if(given_text[i] == '&') {
						i++;
						return AND;
					}
				}
				else if(given_text[i] == '|') {
					i++;
					if(given_text[i] == '|') {
						i++;
						return OR;
					}
				}
				else if(given_text[i] == '!') {
					//std::cout << "Eu sunt i in !: " << i << "ch = ( " << given_text[i] << ")\n";
					i++;
					if(given_text[i] == '=') {
						i++;
						return NOTEQ;
					}
					else {
						return NOT;
					}
				}
				else if(given_text[i] == '<') {
					i++;
					if(given_text[i] == '=') {
						i++;
						return LESSEQ;
					}
					else {
						return LESS;
					}
				}
				else if(given_text[i] == '>') {
					i++;
					if(given_text[i] == '=') {
						i++;
						return GREATEREQ;
					}
					else {
						return GREATER;
					}
				}
				else if(isdigit(given_text[i]) && given_text[i] != '0') {
					i++;
					state = 32;
				}
				else if(given_text[i] == '\'') {
					i++;
					if(given_text[i] != '\'') {
						i++;
					}
					else {
						i++;
						return 47;
					}
				}
				else if (isalpha(given_text[i]) || given_text[i] == '_'){
					state = 51;
					start = i++;
				}
				break;
			case 2:
				break;
			case(28):
				break;
			case(32):
				if(isdigit(given_text[i])) {
					i++;
				}
				return 32;
				break;
			case(51):
				if (isalnum(given_text[i]) || given_text[i] == '_'){
					i++;
				}
				else {
					state = 52;
				}
				break;
			case(ID):
				return ID;
			default:
				printf("Given state doesn't exist : %d (character = %c)\n", state, given_text[i]);
			}
	}
}

std::vector<char> LexicalAnalyzer::extract(int start, const int& end) {
        std::vector<char> arr;
        while(start < end) {
            arr.push_back(given_text[start]);
        }
        return arr;
}

void LexicalAnalyzer::get_all_tokens() {
	auto x = get_next_token();
	while(x != -1) {
		std::cout << x << "\n";
		x = get_next_token();
	}
}

LexicalAnalyzer::LexicalAnalyzer(const std::vector<char>& text) 
: given_text(text),
i(0),
line(0)
{
}
