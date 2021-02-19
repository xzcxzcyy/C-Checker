#include <vector>
#include "token.hpp"
#include "lexer.hpp"

void Lexer::analyze() {
    tokens.clear();
    int state = 0;
    int line = 1;
    char ch;
    bool loopFlag = true;
    std::string word;
    while (loopFlag) {
        ch = inputStream.get();
        switch (state) {
        case 0:
            if (isUnderscore(ch) || isLetter(ch)) {
                word.push_back(ch);
                state = 1;
            } else if (ch == '0') {
                word.push_back(ch);
                state = 2;
            } else if (ch >= '1' && ch <= '9') {
                word.push_back(ch);
                state = 9;
            } else if (ch == '.') {
                word.push_back(ch);
                state = 10;
            } else if (ch == '\'') {
                word.push_back(ch);
                state = 11;
            } else if (ch == '(') {
                tokens.emplace_back("(", Token::OPEN_PAREN);
            } else if (ch == ')') {
                tokens.emplace_back(")", Token::CLOSE_PAREN);
            } else if (ch == '[') {
                tokens.emplace_back("[", Token::OPEN_BRACKET);
            } else if (ch == ']') {
                tokens.emplace_back("]", Token::CLOSE_BRACKET);
            } else if (ch == '{') {
                tokens.emplace_back("{", Token::OPEN_BRACE);
            } else if (ch == '}') {
                tokens.emplace_back("}", Token::CLOSE_BRACE);
            } else if (ch == '+') {
                tokens.emplace_back("+", Token::PLUS);
            } else if (ch == '-') {
                tokens.emplace_back("-", Token::MINUS);
            } else if (ch == '*') {
                tokens.emplace_back("*", Token::TIMES);
            } else if (ch == '/') {
                tokens.emplace_back("/", Token::DIVIDE);
            } else if (ch == ';') {
                tokens.emplace_back(";", Token::SEMICOL);
            } else if (ch == ',') {
                tokens.emplace_back(",", Token::COMMA);
            } else if (isWhiteSpace(ch)) {
                if (ch == '\n') {
                    line++;
                }
            } else if (ch == '%') {
                tokens.emplace_back("%", Token::MOD);
            } else if (ch == '>') {
                word.push_back(ch);
                state = 14;
            } else if (ch == '<') {
                word.push_back(ch);
                state = 15;
            } else if (ch == '=') {
                word.push_back(ch);
                state = 16;
            } else if (ch == '!') {
                word.push_back(ch);
                state = 17;
            } else if (ch == '&') {
                word.push_back(ch);
                state = 18;
            } else if (ch == '|') {
                word.push_back(ch);
                state = 19;
            } else if (isEof(ch)) {
                loopFlag = false;
            } else {
                std::string errorMsg = "Lexical error: illegal character at line ";
                errorMsg.append(std::to_string(line));
                tokens.emplace_back(errorMsg, Token::ERROR_TOKEN);
                errorCount++;
                panic(loopFlag);
            }
            break;

        case 1:
            if (isUnderscore(ch) || isLetter(ch) || isDigit(ch)) {
                word.push_back(ch);
            } else {
                inputStream.unget();
                if (keywords.count(word)) {
                    tokens.emplace_back(word, keywords[word]);
                } else {
                    tokens.emplace_back(word, Token::IDENTIFIER);
                }
                word.clear();
                state = 0;
            }
            break;

        case 2:
            
        }
    }
}

bool Lexer::isUnderscore(char c) {
    return c == '_';
}

bool Lexer::isLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool Lexer::isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool Lexer::isEof(char c) {
    return c == EOF;
}

bool Lexer::isWhiteSpace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

void Lexer::inflateKeywords() {
    keywords["void"] = Token::VOID;
    keywords["int"] = Token::INT;
    keywords["char"] = Token::CHAR;
    keywords["short"] = Token::SHORT;
    keywords["long"] = Token::LONG;
    keywords["double"] = Token::DOUBLE;
    keywords["float"] = Token::FLOAT;
    keywords["const"] = Token::CONST;
    keywords["while"] = Token::WHILE;
    keywords["for"] = Token::FOR;
    keywords["continue"] = Token::CONTINUE;
    keywords["break"] = Token::BREAK;
    keywords["if"] = Token::IF;
    keywords["else"] = Token::ELSE;
    keywords["return"] = Token::RETURN;
}

void Lexer::panic(bool& loopFlag) {
    char pch;
    while (loopFlag) {
        if (pch == '\n') {
            inputStream.unget();
            break;
        } else if (pch == EOF) {
            loopFlag = false;
            break;
        }
    }
}
