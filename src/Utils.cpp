
#include "Utils.hpp"
#include "LexicalAnalyzer.hpp"
#include "TokenList.hpp"
#include "Token.hpp"
#include "Node.hpp"

namespace utils {

    std::vector<char> read_file(const std::string& file_name) {
        std::vector<char> arr;
        char tmp;

        std::ifstream in(file_name);
        if(!in) {
            std::cout << "There was an error while opening the file with name: " <<
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
        std::cout << "Testing " << file_name << ":\n";
        auto arr = read_file(file_name);

        // int i = 0;
        // for(auto const &x: arr) {
        //     std::cout << i << " " << x << "    ";
        //     ++i;
        // }
        // std::cout << "\n\n";

        LexicalAnalyzer lexic_int{arr};
        lexic_int.get_all_tokens(); 
    }

    void test_diagrams() {
     
        test_file("../testing_files/dfa/ct_int.txt");
        test_file("../testing_files/dfa/ct_real.txt");
        test_file("../testing_files/dfa/ct_string.txt");
        test_file("../testing_files/dfa/ct_char.txt");
        test_file("../testing_files/dfa/delimiters+operators.txt");
        test_file("../testing_files/dfa/line_comment.txt");
        test_file("../testing_files/dfa/block_comment.txt");
    }
}
