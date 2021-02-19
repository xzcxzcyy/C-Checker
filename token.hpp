#ifndef TOKEN_HPP
#define TOKEN_HPP
#include <string>

class Token {
public:
    enum TokenType {
        ERROR_TOKEN,
        VOID,
        INT,
        CHAR,
        SHORT,
        LONG,
        DOUBLE,
        FLOAT,
        CONST,
        WHILE,
        FOR,
        CONTINUE,
        BREAK,
        IF,
        ELSE,
        IDENTIFIER,
        RETURN,
        CONST_INT,
        CONST_LONG,
        CONST_FLOAT,
        CONST_CHAR,
        OPEN_PAREN,
        CLOSE_PAREN,
        OPEN_BRACKET,
        CLOSE_BRACKET,
        OPEN_BRACE,
        CLOSE_BRACE,
        PLUS,
        MINUS,
        TIMES,
        DIVIDE,
        GREATER,
        GEQUAL,
        LESS,
        LEQUAL,
        EQUAL,
        NOT_EQUAL,
        LOGICAL_AND,
        LOGICAL_OR,
        SEMICOL,
        COMMA,
        WHITESPACE,
        ASSIGN,
        COMMENTS,
    };
    TokenType type;
    std::string name;

    Token(std::string name, TokenType type) {
        this->name = name;
        this->type = type;
    }
};

#endif