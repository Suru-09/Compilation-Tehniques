#ifndef TOKEN_HPP
#define TOKEN_HPP

#include<iostream>
#include<vector>

class Token {
    private:
        int code;
        union value {
            std::vector<char> text;
            long int i;
            double r;
        };
        int line;
public:
    Token() = default;
};

#endif