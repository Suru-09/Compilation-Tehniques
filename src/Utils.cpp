
#include "Utils.hpp"
#include "LexicalAnalyzer.hpp"
#include "SyntacticAnalyzer.hpp"
#include "Logger.hpp"
#include "SymbolTable.hpp"
#include "Type.hpp"
#include "Symbol.hpp"
#include "VirtualMachine.hpp"
#include "InstructionList.hpp"
#include "Instruction.hpp"
namespace utils {

    std::vector<char> read_file(const std::string& file_name) {
        Logger logger{"Utils"};
        std::vector<char> arr;
        char tmp;

        std::ifstream in(file_name);
        if(!in) {
            std::cout << logger << "There was an error while opening the file with name: " <<
                file_name;
        }

        while(1) {
            in.get(tmp);
            arr.push_back(tmp);
            if(in.eof())
                break;
            tmp = 0;
        }
        
        arr.push_back(EOF);
        in.close();
        return arr;
    }

    void test_file(const std::string& file_name) {
        Logger logger{"Utils"};
        std::cout << logger << "Testing " << file_name << ":\n";
        auto arr = read_file(file_name);

        LexicalAnalyzer lexic_int{arr};
        lexic_int.get_all_tokens(); 

        SyntacticAnalyzer syn{lexic_int};
        std::cout << logger << "Starting the syntactic analysis: \n";
        syn.unit();

        syn.test_vm();
    }

    void test_diagrams() {
     
        // test_file("../testing_files/dfa/ct_int.txt");
        // test_file("../testing_files/dfa/ct_real.txt");
        // test_file("../testing_files/dfa/ct_string.txt");
        // test_file("../testing_files/dfa/ct_char.txt");
        // test_file("../testing_files/dfa/delimiters+operators.txt");
        // test_file("../testing_files/dfa/line_comment.txt");
        // test_file("../testing_files/dfa/block_comment.txt");
        test_file("../testing_files/main.c");
        // test_file("../testing_files/input.txt");
        // test_file("../testing_files/tests/0.c");
        // test_file("../testing_files/tests/1.c");
        // test_file("../testing_files/tests/2.c");
        // test_file("../testing_files/tests/3.c");
        // test_file("../testing_files/tests/4.c");
        // test_file("../testing_files/tests/5.c");
        // test_file("../testing_files/tests/6.c");
        // test_file("../testing_files/tests/7.c");
        // test_file("../testing_files/tests/8.c");
        // test_file("../testing_files/tests/9.c");
    }

    void test_symbol_structure() {
        SymbolTable table;

        Logger logger{"Utils"};
        std::cout << logger << "Testing " << "input.txt" << ":\n";
        auto arr = read_file("../testing_files/tests/0.c");

        LexicalAnalyzer lexic_int{arr};
        lexic_int.get_all_tokens(); 

        auto list = lexic_int.token_list.get_head();
        while(list) {
            // std:: cout << logger << "Eu sunt: " << list->token.code << "\n";
            Type type(list->token.code % 5);
            Symbol symbol("E mare nume", list->token.code, type);
            table.add_symbol(symbol);
            list = list->next;
        }

        Symbol symb = table.find_symbol("E mare nume");
        std::cout << logger << "Eu sunt numele simbolului gasit: " << symb.get_name() << "\n";
        table.delete_symbol("E mare nume");

        table.print_symbol_table();
    }

    std::string log_error(const int& line, const std::string& message) {
        std::string tmp;
        tmp = message + " at line: " + std::to_string(line) + "\n";
        return tmp;
    }

    std::string class_to_string(int class_) {
        Logger logger{"Utils"};
        switch(class_) {
            case 0:
                return "VARIABLE";
            case 1:
                return "FUNCTION";
            case 2:
                return "EXTERNAL FUNCTION";
            case 3:
                return "STRUCT";
            default:
                std::cout << logger << "This value doesn;t exists in [CLASS] to string!!\n";
                break;
        }

        return "NOT A CLASS";
    }
    std::string type_to_string(int type) {
        Logger logger{"Utils"};
        switch(type) {
            case 0:
                return "TB_INT";
            case 1:
                return "TB_DOUBLE";
            case 2:
                return "TB_CHAR";
            case 3:
                return "TB_STRING";
            case 4:
                return "TB_STRUCT";
            case 5:
                return "TB_VOID";
            default:
                std::cout << logger << "This value doesn;t exists in [TYPE] to string!!\n";
                break;
        }
        return "NOT A TYPE";     
    }
}
