#include "LexicalAnalyzer.hpp"
#include "Token.hpp"
#include "TokenList.hpp"
#include "Node.hpp"


#include <string>


int LexicalAnalyzer::get_next_token() {
    int state = 0, start = 0;
	int count = 0;
	std::string id_text;

    while(1){
		count++;
		// if(count > 15)
		// 	return -1;

		// std::cout << "I am i: " << i << " size: " << given_text.size() - 1 << "\n";
		// std::cout << "I am state : " << state << " with ch = (" << given_text[i] << ")\n";
		switch (state){
			case(0):
				if (given_text[i] == ' ' || given_text[i] == '\t' ||
					given_text[i] == '\r' || given_text[i] == '\n'){
					
					if (given_text[i] == '\n') {
						line++;
					}
					i++;
				}
				else if(given_text[i] == '\0') {
					return END;
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
					else if(given_text[i] == '*') {
						++i;
						state = 29;
					}
					else {
						return DIV;
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
				else if(given_text[i] == '0') {
					i++;
					state = 31;
				}
				else if(isdigit(given_text[i]) && given_text[i] != '0') {
					i++;
					state = 32;
				}
				else if(given_text[i] == '=') {
					i++;
					if(given_text[i] == '=') {
						++i;
						return EQUAL;
					}
					else {
						return ASSIGN;
					}
				}
				else if(given_text[i] == '\'') {
					i++;
					if(given_text[i] == '\\') {
						++i;
						state = 49;
					}
					else if(given_text[i] != '\\' && given_text[i] != '\'') {
						++i;
						state = 50;
					}
				}
				else if(given_text[i] == '"') {
					++i;
					state = 52;
				}
				else if (isalpha(given_text[i]) || given_text[i] == '_'){
					state = 56;
					start = i++;
				}
				break;
			case(28):
				if(given_text[i] != '\n' && given_text[i] != '\r'
					&& given_text[i] != '\0') {
					
					++i;
				}
				else {
					state = 0;
				}
				break;
			case(29):
				if(given_text[i] == '*') {
					i++;
					state = 30;
				}
				else {
					i++;
				}
				break;
			case(30):
				if(given_text[i] != '*' && given_text[i] != '/') {
					++i;
					state = 29;
				}
				else if(given_text[i] == '*') {
					++i;
				}
				else if(given_text[i] == '/'){
					++i;
					state = 0;
				}
				break;
			case(31):
				if(isdigit(given_text[i]) && given_text[i] <= '7') {
					i++;
					state = 33;
				}
				else if(isdigit(given_text[i]) &&  given_text[i] >= '8') {
					i++;
					state = 37;
				}
				else if(given_text[i] == 'x' || given_text[i] == 'X') {
					i++;
					state = 34;
				}
				else if(given_text[i] == '.') {
					i++;
					state = 38;
				}
				else {
					return -1;
				}
				break;
			case(32):
				if(isdigit(given_text[i])) {
					++i;
				}
				else if(given_text[i] == 'e' || given_text[i] == 'E') {
					++i;
					state = 40;
				}
				else if(given_text[i] == '.') {
					++i;
					state = 38;
				}
				else {
					state = CT_INT;
				}
				break;
			case(33):
				if(isdigit(given_text[i]) && given_text[i] <= '7') {
					++i;
				}
				else if(given_text[i] == 'e' || given_text[i] == 'E') {
					++i;
					state = 40;
				}
				else {
					state = CT_INT;
				}
				break;
			case(34):
				if(isdigit(given_text[i]) || (given_text[i] >= 'a' && given_text[i] <= 'f')
					|| (given_text[i] >= 'A' && given_text[i] <= 'F') ) {
					
					++i;
					state = 35;
				}
				break;
			case(35):
				if(isdigit(given_text[i]) || (given_text[i] >= 'a' && given_text[i] <= 'f')
				|| (given_text[i] >= 'A' && given_text[i] <= 'F') ) {
				
				++i;
				}
				else {
					state = CT_INT;
				}
				break;
			case(CT_INT):
				return CT_INT;
			case(37):
				if( isdigit(given_text[i]) ) {
					i++;
				}
				else if(given_text[i] == 'e' || given_text[i] == 'E') {
					++i;
					state = 40;
				}
				else if(given_text[i] == '.') {
					i++;
					state = 38;
				}
				else {
					return -1;
				}
				break;
			case(38):
				if( isdigit(given_text[i])) {
					i++;
					state = 39;
				}
				break;
			case(39):
				if(isdigit(given_text[i])) {
					i++;
				}
				else if(given_text[i] == 'e' || given_text[i] == 'E') {
					i++;
					state = 40;
				}
				else {
					state = CT_REAL;
				}
				break;
			case(40):
				if(isdigit(given_text[i])) {
					++i;
					state = 41;
				}
				else if(given_text[i] == '+' || given_text[i] == '-') {
					++i;
					state = 42;
				}
				break;
			case(41):
				if(isdigit(given_text[i])) {
					++i;
				}
				else {
					state = CT_REAL;
				}
				break;
			case(42):
				if(isdigit(given_text[i])) {
					++i;
					state = 43;
				}
				break;
			case(43):
				if(isdigit(given_text[i])) {
					++i;
				}
				else {
					state = CT_REAL;
				}
				break;
			case(CT_REAL):
				return CT_REAL;
			case(49):
				if(given_text[i] == 'a' || given_text[i] == 'b' ||
					given_text[i] == '?' || given_text[i] == 'f'||
					given_text[i] == 'n' || given_text[i] == 'n' ||
					given_text[i] == 't' || given_text[i] == 'v' ||
					given_text[i] == '"' || given_text[i] == '\\' ||
					given_text[i] == '\0') {

					++i;
					state = 50;
				}
				break;
			case 50:
				if(given_text[i] == '\'') {
					++i;
					state = CT_CHAR;
				}
				break;
			case(52):
				if(given_text[i] == '\\') {
					i++;
					state = 53;
				}
				else if(given_text[i] != '"') {
					++i;
					state = 54;
				}
				break;
			case(53):
				if(given_text[i] == 'a' || given_text[i] == 'b' ||
					given_text[i] == '?' || given_text[i] == 'f'||
					given_text[i] == 'n' || given_text[i] == 'n' ||
					given_text[i] == 't' || given_text[i] == 'v' ||
					given_text[i] == '"' || given_text[i] == '\\' ||
					given_text[i] == '\0') {

					++i;
					state = 54;
				}
				break;
			case(54):
				if(given_text[i] == '"') {
					++i;
					state  = CT_STRING;
				}
				else {
					state = 52;
				}
				break;
			case(CT_CHAR):
				return CT_CHAR;
			case(CT_STRING):
				return CT_STRING;
			case(56):
				if (isalnum(given_text[i]) || given_text[i] == '_'){
					i++;
				}
				else {
					state = ID;
				}
				break;
			case(ID):
				id_text =  extract(start, i);
				// std::cout << "Textul de la ID: " << id_text << "\n";
				add_token(ID, id_text, line);
				return ID;
				break;
			default:
				printf("Given state doesn't exist : %d (character = %c)\n", state, given_text[i]);
				break;
			}
	}
}

std::string LexicalAnalyzer::extract(int start, const int& end) {
        std::vector<char> arr;
        while(start < end) {
            arr.push_back(given_text[start]);
			start++;
        }
        return std::string(arr.begin(), arr.end());
}

void LexicalAnalyzer::get_all_tokens() {
	int count_tokens = 1;
	auto x = get_next_token();
	std::cout << x << "\n";

	while(1) {
		x = get_next_token();
		if(x == -1)
			break;
		std::cout << x << "\n";
		count_tokens++;
	}

	std::cout << "Number of tokens: " << count_tokens << "\n";
	std::cout << "Eu sunt lista de token-uri: ";
	token_list.print_list();
	std::cout << "\n";
}

LexicalAnalyzer::LexicalAnalyzer(const std::vector<char>& text) 
: given_text(text),
i(0),
line(0)
{
}

void LexicalAnalyzer::add_token(const int& code, const std::string& text, const int& line) {
	Token token{code, text, line};
	Node node{token};
	token_list.push(node);
}

void LexicalAnalyzer::add_token(const int& code, const long int& text, const int& line) {
	Token token{code, text, line};
	Node node{token};
	token_list.push(node);
}

void LexicalAnalyzer::add_token(const int& code, const double& text, const int& line) {
	Token token{code, text, line};
	Node node{token};
	token_list.push(node);
}
