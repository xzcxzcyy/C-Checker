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
        RETURN,
        IDENTIFIER,
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
        MOD,
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
    std::string fileName;
    int line;

    Token(std::string name, TokenType type, std::string fileName, int line) {
        this->name = name;
        this->type = type;
        this->fileName = fileName;
        this->line = line;
    }

    bool operator<(const Token &another) {
        if (this->type < another.type) {
            return true;
        }
        if (this->type > another.type) {
            return false;
        }

        if (this->fileName < another.fileName) {
            return true;
        }
        if (this->fileName > another.fileName) {
            return false;
        }

        if (this->line < another.line) {
            return true;
        }
        if (this->line > another.line) {
            return false;
        }

        if (this->name < another.name) {
            return true;
        }
        if (this->name > another.name) {
            return false;
        }

        return false;
    }
};

#endif