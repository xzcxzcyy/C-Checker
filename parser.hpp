#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include "token.hpp"

class Parser {
private:
    std::vector<Token> tokens;
    std::vector<Token>::iterator tokensIt;

public:
    Parser(std::vector<Token> ts) : tokens(ts) {
        tokensIt = tokens.begin();
    }
};
#endif