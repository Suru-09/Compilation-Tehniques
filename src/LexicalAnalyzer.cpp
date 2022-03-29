#include "LexicalAnalyzer.hpp"
#include "Token.hpp"
#include "TokenList.hpp"
#include "Node.hpp"

#include <string>


int LexicalAnalyzer::get_next_token() {
    int state = 0, start = 0;
	std::string id_text, actual_id_text;

    while(1){
		if(i > given_text.size() - 1) {
			std::cout << logger << "There was an error while processing state = (" << state << ")\n";
			return END;
		}

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
					add_token(END, line);
					return END;
				}
				else if(given_text[i] == ',') {
					i++;
					add_token(COMMA, line);
					return COMMA;
				}
				else if(given_text[i] == ';') {
					i++;
					add_token(SEMICOLON, line);
					return SEMICOLON;
				}
				else if(given_text[i] == '(') {
					i++;
					add_token(LPAR, line);
					return LPAR;
				}
				else if(given_text[i] == ')') {
					i++;
					add_token(RPAR, line);
					return RPAR;
				}
				else if(given_text[i] == '[') {
					i++;
					add_token(LBRACKET, line);
					return LBRACKET;
				}
				else if(given_text[i] == ']') {
					i++;
					add_token(RBRACKET, line);
					return RBRACKET;
				}
				else if(given_text[i] == '{') {
					i++;
					add_token(LACC, line);
					return LACC;
				}
				else if(given_text[i] == '}') {
					i++;
					add_token(RACC, line);
					return RACC;
				}
				else if(given_text[i] == '+') {
					i++;
					add_token(ADD, line);
					return ADD;
				}
				else if(given_text[i] == '-') {
					i++;
					add_token(SUB, line);
					return SUB;
				}
				else if(given_text[i] == '*') {
					i++;
					add_token(MUL, line);
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
						add_token(DIV, line);
						return DIV;
					}
				}
				else if(given_text[i] == '.') {
					i++;
					add_token(DOT, line);
					return DOT;
				}
				else if(given_text[i] == '&') {
					i++;
					if(given_text[i] == '&') {
						i++;
						add_token(AND, line);
						return AND;
					}
				}
				else if(given_text[i] == '|') {
					i++;
					if(given_text[i] == '|') {
						i++;
						add_token(OR, line);
						return OR;
					}
				}
				else if(given_text[i] == '!') {
					//std::cout << "Eu sunt i in !: " << i << "ch = ( " << given_text[i] << ")\n";
					i++;
					if(given_text[i] == '=') {
						i++;
						add_token(NOTEQ, line);
						return NOTEQ;
					}
					else {
						add_token(NOT, line);
						return NOT;
					}
				}
				else if(given_text[i] == '<') {
					i++;
					if(given_text[i] == '=') {
						i++;
						add_token(LESSEQ, line);
						return LESSEQ;
					}
					else {
						add_token(LESS, line);
						return LESS;
					}
				}
				else if(given_text[i] == '>') {
					i++;
					if(given_text[i] == '=') {
						i++;
						add_token(GREATEREQ, line);
						return GREATEREQ;
					}
					else {
						add_token(GREATER, line);
						return GREATER;
					}
				}
				else if(given_text[i] == '0') {
					start = i;
					i++;
					state = 31;
				}
				else if(isdigit(given_text[i]) && given_text[i] != '0') {
					start = i;
					i++;
					state = 32;
				}
				else if(given_text[i] == '=') {
					i++;
					if(given_text[i] == '=') {
						++i;
						add_token(EQUAL, line);
						return EQUAL;
					}
					else {
						add_token(ASSIGN, line);
						return ASSIGN;
					}
				}
				else if(given_text[i] == '\'') {
					start = i;
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
					start = i;
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
					state = CT_INT;
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
				add_token(CT_INT, extract(start, i), line);
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
				// std::cout << "CT_REAL: " <<  stod(extract(start, i)) << "\n";
				add_token(CT_REAL, std::stod(extract(start, i)), line);
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
				else if(given_text[i] == '"') {
					++i;
					state = CT_STRING;
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
				add_token(CT_CHAR, extract(start, i), line);
				return CT_CHAR;
			case(CT_STRING):
				add_token(CT_STRING, extract(start, i), line);
				return CT_STRING;
			case(56):
				if (isdigit(given_text[i]) || isalnum(given_text[i]) || given_text[i] == '_'){
					i++;
				}
				else {
					state = ID;
				}
				break;
			case(ID):
				actual_id_text = extract(start, i);
				id_text = return_keyword(actual_id_text);

				if( id_text == "NONE") {
					add_token(ID, actual_id_text, line);
				}

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
	// std::cout << x << "\n";

	while(1) {
		x = get_next_token();
		// std::cout << x << "\n";
		if(x == -1)
			break;
		count_tokens++;
	}

	// std::cout << "Number of tokens: " << count_tokens << "\n";
	std::cout << logger << "Eu sunt lista de token-uri: \n";
	token_list.print_list();
	std::cout << "\n";
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

void LexicalAnalyzer::add_token(const int& code, const int& line) {
	Token token{code, line};
	Node node{token};
	token_list.push(node);
}

std::string LexicalAnalyzer::return_keyword(const std::string &str) {
	std::string copy = str;
	for(auto &c : copy)
		c = toupper(c);
	
	if(copy.compare("BREAK") == 0) {
		add_token(BREAK, str, line);
		return std::string{"BREAK"};
	}
	else if(copy.compare("CHAR") == 0) {
		add_token(CHAR, str, line);
		return std::string{"CHAR"};
	}
	else if(copy.compare("DOUBLE") == 0) {
		add_token(DOUBLE, str, line);
		return std::string{"DOUBLE"};
	}
	else if(copy.compare("ELSE") == 0) {
		add_token(ELSE, str, line);
		return std::string{"ELSE"};
	}
	else if(copy.compare("FOR") == 0) {
		add_token(FOR, str, line);
		return std::string{"FOR"};
	}
	else if(copy.compare("INT") == 0) {
		add_token(INT, str, line);
		return std::string{"INT"};
	}
	else if(copy.compare("IF") == 0) {
		add_token(IF, str, line);
		return std::string{"IF"};
	}
	else if(copy.compare("RETURN") == 0 ){
		add_token(RETURN, str, line);
		return std::string{"RETURN"};
	}
	else if(copy.compare("STRUCT") == 0) {
		add_token(STRUCT, str, line);
		return std::string{"STRUCT"};
	}
	else if(copy.compare("VOID") == 0) {
		add_token(VOID, str, line);
		return std::string{"VOID"};
	}
	else if(copy.compare("WHILE") == 0 ){
		add_token(WHILE, str, line);
		return std::string{"WHILE"};
	}
	else {
		return std::string{"NONE"};
	}
}

void LexicalAnalyzer::init_map() {
	pretty_map[COMMA] = "COMMA";
	pretty_map[SEMICOLON] = "SEMICOLON";
	pretty_map[LPAR] = "LPAR";
	pretty_map[RPAR] = "RPAR";
	pretty_map[LBRACKET] = "LBRACKET";
	pretty_map[RBRACKET] = "RBRACKET";
	pretty_map[LACC] = "LACC";
	pretty_map[RACC] = "RACC";
	pretty_map[ADD] = "ADD";
	pretty_map[MUL] = "MUL";
	pretty_map[SUB] = "SUB";
	pretty_map[DOT] = "DOT";
	pretty_map[AND] = "AND";
	pretty_map[OR] = "OR";
	pretty_map[NOTEQ] = "NOTEQ";
	pretty_map[NOT] = "NOT";
	pretty_map[LESSEQ] = "LESSEQ";
	pretty_map[LESS] = "LESS";
	pretty_map[GREATEREQ] = "GREATEREQ";
	pretty_map[GREATER] = "GREATER";
	pretty_map[DIV] = "DIV";
	pretty_map[CT_INT] = "CT_INT";
	pretty_map[CT_REAL] = "CT_REAL";
	pretty_map[EQUAL] = "EQUAL";
	pretty_map[ASSIGN] = "ASSIGN";
	pretty_map[CT_CHAR] = "CT_CHAR";
	pretty_map[CT_STRING] = "CT_STRING";
	pretty_map[ID] = "ID";
	pretty_map[BREAK] = "BREAK";
	pretty_map[CHAR] = "CHAR";
	pretty_map[DOUBLE] = "DOUBLE";
	pretty_map[ELSE] = "ELSE";
	pretty_map[FOR] = "FOR";
	pretty_map[INT] = "INT";
	pretty_map[IF] = "IF";
	pretty_map[RETURN] = "RETURN";
	pretty_map[STRUCT] = "STRUCT";
	pretty_map[VOID] = "VOID";
	pretty_map[WHILE] = "WHILE";
	pretty_map[END] = "END";
}

LexicalAnalyzer::LexicalAnalyzer(const std::vector<char>& text) 
: given_text(text),
i(0),
line(0),
class_name("LexicalAnalyzer")
{	
	logger = Logger{class_name};
	init_map();
}

LexicalAnalyzer::LexicalAnalyzer()
: class_name("LexicalAnalyzer")
{	
	logger = Logger{class_name};
	init_map();
}

std::string LexicalAnalyzer::print_pretty(int id) {
	return pretty_map[TYPES(id)];
}
