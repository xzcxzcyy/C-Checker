#ifndef PARSER_HPP
#define PARSER_HPP

#include <utility>
#include <vector>
#include "token.hpp"

class Parser {
private:
    std::vector<Token> tokens;
    std::vector<Token>::iterator tokensIt;

public:
    explicit Parser(std::vector<Token> ts) : tokens(std::move(ts)) {
        tokensIt = tokens.begin();
    }
};

#endif